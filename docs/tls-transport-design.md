# 客户端探针（Agent）加密传输方案

## 1. 概述

客户端探针（Agent）基于 fluent-bit 裁剪构建，运行在 ARM 嵌入式/车载 Linux 环境中，以 daemon 方式长期运行。Agent 通过 **TCP 长连接 + TLS 加密** 与服务端通信，将采集到的数据安全、可靠地传输至服务端。

本文档描述客户端与服务端之间的加密传输方案，供双方技术团队对齐实现细节。

## 2. 传输架构

### 2.1 协议栈

```
+---------------------+
|   数据帧 (JSON/MsgPack)  |   应用层：序列化后的业务数据
+---------------------+
|       TLS 1.2+      |   安全层：传输加密与身份认证
+---------------------+
|        TCP           |   传输层：可靠的长连接传输
+---------------------+
```

### 2.2 连接模式

- **长连接**：客户端主动发起 TCP 连接并完成 TLS 握手，连接建立后持续复用
- **连接方向**：始终由客户端发起连接，服务端被动监听
- **数据方向**：客户端向服务端单向推送数据

### 2.3 端口约定

| 角色 | 端口 | 说明 |
|------|------|------|
| 服务端 | **5170**（建议） | TLS 专用监听端口，fluent-bit TCP 输出插件的默认端口 |
| 客户端 | 随机 | 由操作系统分配 |

> 端口号可根据实际部署调整，客户端通过配置文件指定服务端地址和端口。

## 3. TLS 配置

### 3.1 TLS 版本

| 项目 | 要求 |
|------|------|
| 最低版本 | **TLS 1.2** |
| 推荐版本 | **TLS 1.3** |
| 禁用���本 | TLS 1.0、TLS 1.1、SSLv3 及更低版本 |

客户端通过 `tls.min_version` 配置项控制最低 TLS 版本。

### 3.2 认证模式

**当前阶段：单向认证（Server Authentication）**

- 服务端部署服务端证书和私钥
- 客户端部署 CA 根证书，用于验证服��端身份
- 客户端无需提供自身证书

**预留阶段：双向认证（mTLS / Mutual TLS）**

- 在单向认证基础上，客户端额外部署客户端证书和私钥
- 服务端需配置为要求并验证客户端证书
- 启用时客户端只需取消注释 `tls.crt_file` 和 `tls.key_file` 配置项

### 3.3 推荐密码套件

TLS 1.2 推荐密码套件（按优先级排列）：

```
ECDHE-RSA-AES256-GCM-SHA384
ECDHE-RSA-AES128-GCM-SHA256
ECDHE-ECDSA-AES256-GCM-SHA384
ECDHE-ECDSA-AES128-GCM-SHA256
```

TLS 1.3 使用协议内置的密码套件，无需额外配置：

```
TLS_AES_256_GCM_SHA384
TLS_AES_128_GCM_SHA256
TLS_CHACHA20_POLY1305_SHA256
```

客户端可通过 `tls.ciphers` 配置项指定 TLS 1.2 密码套件。若使用 TLS 1.3，密码套件由协议自动协商。

### 3.4 客户端配置示例

以下为 fluent-bit 配置文件中 TCP 输出插件的完整 TLS 配置：

```ini
[OUTPUT]
    Name              tcp
    Host              server.example.com
    Port              5170
    Format            json

    # TLS 基础配置
    tls               on
    tls.verify        on
    tls.ca_file       /etc/agent/certs/ca.crt

    # TLS 版本控制（可选，建议显式配置）
    tls.min_version   tlsv1.2

    # TLS 密码套件（可选，使用默认即可）
    # tls.ciphers     ECDHE-RSA-AES256-GCM-SHA384:ECDHE-RSA-AES128-GCM-SHA256

    # TLS 调试级别（可选，0=关闭 1=错误 2=状态 3=信息 4=详细）
    # tls.debug       1

    # SNI 主机名（可选，默认使用 Host 值）
    # tls.vhost       server.example.com

    # 主机名验证（可选）
    # tls.verify_hostname on

    # mTLS 双向认证预留（启用时取消注释）
    # tls.crt_file    /etc/agent/certs/client.crt
    # tls.key_file    /etc/agent/certs/client.key
    # tls.key_passwd  <私钥密码，如私钥无密码则不需要>
```

### 3.5 客户端支持的全部 TLS 配置项

以下配置项来源于 `src/tls/flb_tls.c` 中的 `tls_configmap` 定义：

| 配置项 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| `tls` | Bool | `off` | 启用或禁用 TLS |
| `tls.verify` | Bool | `on` | 启用证书验证 |
| `tls.debug` | Int | `1` | TLS 调试级别（0-4） |
| `tls.ca_file` | String | 无 | CA 证书文件绝对路径 |
| `tls.ca_path` | String | 无 | CA 证书目录扫描路径 |
| `tls.crt_file` | String | 无 | 客户端证书文件绝对路径（mTLS） |
| `tls.key_file` | String | 无 | 客户端私钥文件绝对路径（mTLS） |
| `tls.key_passwd` | String | 无 | 私钥文件密码（可选） |
| `tls.vhost` | String | 无 | TLS SNI 扩展的主机名 |
| `tls.verify_hostname` | Bool | `off` | 启用主机名验证 |
| `tls.min_version` | String | 无 | TLS 最低版本 |
| `tls.max_version` | String | 无 | TLS 最高版本 |
| `tls.ciphers` | String | 无 | TLS 1.2 密码套件列表 |

## 4. 证书体系

### 4.1 信任链结构

采用**自建私有 CA**（Private Certificate Authority）签发证书。

**单向认证（当前）：**

```
Root CA（自建）
  └── Server Certificate（服务端证书）
```

**双向认证（mTLS 预留）：**

```
Root CA（自建）
  ├── Server Certificate（服务端证书）
  └── Client Certificate（客户端证书）
```

### 4.2 证书格式要求

所有证书和密钥文件均使用 **PEM** 格式：

| 文件类型 | 格式 | 扩展名 | 说明 |
|----------|------|--------|------|
| CA 根证书 | PEM | `.crt` 或 `.pem` | Base64 编码的 X.509 证书 |
| 服务端证书 | PEM | `.crt` 或 `.pem` | 由 CA 签发的服务端证书 |
| 服务端私钥 | PEM | `.key` | RSA 或 ECDSA 私钥 |
| 客户端证书 | PEM | `.crt` 或 `.pem` | 由 CA 签发的客户端证书（mTLS） |
| 客户端私钥 | PEM | `.key` | RSA 或 ECDSA 私钥（mTLS） |

### 4.3 服务端需提供的证书文件

| 文件 | 用途 | 提供给 |
|------|------|--------|
| `ca.crt` | CA 根证书 | 客户端（用于验证服务端身份） |
| `server.crt` | 服务端证书 | 服务端自身部署 |
| `server.key` | 服务端私钥 | 服务端自身部署 |

### 4.4 客户端需部署的文件

**单向认证（当前）：**

| 文件 | 部署路径（建议） | 来源 |
|------|------------------|------|
| `ca.crt` | `/etc/agent/certs/ca.crt` | 服务端团队提供 |

**双向认证（mTLS 启用后额外需要）：**

| 文件 | 部署路径（建议） | 来源 |
|------|------------------|------|
| `client.crt` | `/etc/agent/certs/client.crt` | CA 签发，服务端团队提供 |
| `client.key` | `/etc/agent/certs/client.key` | 与证书配对的私钥 |

> 证书文件权限建议：证书文件 `644`，私钥文件 `600`。

## 5. 数据格式

### 5.1 支持的序列化格式

TCP 输出插件（`out_tcp`）支持以下数据格式，通过 `Format` 配置项指定：

| 格式 | 配置值 | 说明 |
|------|--------|------|
| MsgPack | `msgpack` | 二进制序列化格式，体积小、解析快，**默认值** |
| JSON | `json` | 标准 JSON 数组格式，可读性好 |
| JSON Lines | `json_lines` | 每条记录一行 JSON，便于流式解析 |
| JSON Stream | `json_stream` | 连续 JSON 对象流 |

### 5.2 格式特点对比

| 特性 | MsgPack | JSON / JSON Lines |
|------|---------|-------------------|
| 数据体积 | 小（二进制紧凑编码） | 较大（文本格式） |
| 解析速度 | 快 | 较慢 |
| 可读性 | 不可直接阅读 | 人类可读 |
| 调试便利性 | 需工具解析 | 直接查看 |
| 适用场景 | 生产环境（带宽敏感） | 开发调试、对接标准 API |

### 5.3 格式选择方式

数据格式通过客户端配置文件静态配置，示例：

```ini
# 使用 JSON Lines 格式（推荐对接场景）
[OUTPUT]
    Name     tcp
    Format   json_lines
    ...

# 使用 MsgPack 格式（默认，带宽优化）
[OUTPUT]
    Name     tcp
    Format   msgpack
    ...
```

服务端需根据客户端配置的格式实现对应的数据解析逻辑。建议双方协商确定统一格式后固定使用。

## 6. 服务端对接要求

### 6.1 服务端需完成的工作

1. **开放 TLS 监听端口**
   - 建议端口：`5170`（与 fluent-bit 默认端口一致）
   - 配置 TLS，加载服务端证书（`server.crt`）和私钥（`server.key`）

2. **证书准备**
   - 使用自建 CA 签发服务端证书
   - 将 CA 根证书（`ca.crt`）提供给客户端团队部署
   - 服务端证书的 CN（Common Name）或 SAN（Subject Alternative Name）应与客户端连接的目标主机名匹配

3. **TLS 版本与密码套件**
   - 最低支持 TLS 1.2
   - 禁用 TLS 1.0 / 1.1 / SSLv3
   - 配置推荐的密码套件（参见第 3.3 节）

4. **数据解析**
   - 根据协商的数据格式（JSON 或 MsgPack）实现解析逻辑
   - TCP 长连接场景下，需处理粘包/拆包
   - 若使用 `json_lines` 格式，以换行符 `\n` 分隔每条记录

5. **连接管理**
   - 支持 TCP 长连接，避免频繁断开
   - 处理客户端的断线重连

### 6.2 TLS 握手流程

```
��户端 (Agent)                          服务端
    |                                     |
    |-------- ClientHello --------------->|  1. 客户端发起连接，声明支持的 TLS 版本和密码套件
    |                                     |
    |<------- ServerHello ----------------|  2. 服务端选择 TLS 版本和密码套件
    |<------- Server Certificate ---------|  3. 服务端发送证书
    |<------- (CertificateRequest) -------|  4. (mTLS 时) 服务端请求客户端证书
    |<------- ServerHelloDone ------------|
    |                                     |
    |  [客户端验证服务端证书，使用 ca.crt]    |  5. 客户端用 CA 证书验证服务端身份
    |                                     |
    |-------- (Client Certificate) ------>|  6. (mTLS 时) 客户端发送证书
    |-------- ClientKeyExchange --------->|  7. 密钥交换
    |-------- Finished ------------------>|
    |                                     |
    |<------- Finished -------------------|  8. 握手完成
    |                                     |
    |======== 加密数据传输 ================|  9. 开始传输业务数据
```

> 以上为 TLS 1.2 握手流程示意。TLS 1.3 握手步骤更少（1-RTT），但整体逻辑一致。

## 7. 安全性说明

### 7.1 传输层加密已足够

TLS 传输层加密提供以下安全保障，满足当前场景需求：

| 安全特性 | TLS 提供的保障 |
|----------|---------------|
| **机密性** | 对称加密（AES-GCM）保护数据内容不被窃听 |
| **完整性** | HMAC/AEAD 机制确保数据未被篡改 |
| **身份认证** | 证书体系确认通信对方身份 |
| **防重放** | TLS 记录层序列号机制防止重放攻击 |
| **前向保密** | ECDHE 密钥交换确保历史会话安全 |

因此，**不需要在应用层额外实现加密**。应用层只需关注数据的序列化和业务逻辑。

### 7.2 后续安全增强路径

当安全需求升级时，可沿以下路径逐步增强：

1. **启用 mTLS 双向认证**
   - 客户端部署客户端证书和私钥
   - 服务端配置为验证客户端证书
   - 客户端配置取消注释 `tls.crt_file` 和 `tls.key_file`
   - 实现设备级身份认证，防止未授权设备接入

2. **启用主机名验证**
   - 客户端配置 `tls.verify_hostname on`
   - 服务端证书 SAN 需包含实际连接的主机名或 IP
   - 防止中间人攻击

3. **证书轮换机制**
   - 定期更新服务端证书和客户端证书
   - 客户端支持证书热更新（重启生效）
