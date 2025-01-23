# Estrutura `sockaddr_in` e Parâmetros de Configuração

### Estrutura `sockaddr_in`
Contém um endereço de internet.

---

# Estrutura `addrinfo`

A estrutura `addrinfo` é usada para armazenar informações sobre endereços de rede e configurações de sockets.

---

### Parâmetro `ai_family`
Define a família de endereços:

- `AF_UNSPEC`: Aceita qualquer família de endereços.
- `AF_INET`: IPv4.
- `AF_INET6`: IPv6.

---

### Parâmetro `ai_socktype`
Define o tipo de socket:

- `SOCK_STREAM`: TCP.
- `SOCK_DGRAM`: UDP.
- `SOCK_RAW`: Socket bruto.
- `SOCK_SEQPACKET`: Fluxo de pacotes sequenciado.
- `SOCK_RDM`: Mensagem entregue de forma confiável.
- `SOCK_PACKET`: Método específico do Linux para capturar pacotes no nível da interface.

---

### Configuração `hints.ai_flags`
Define flags para configurações específicas:

- `AI_PASSIVE`: Usado para chamadas `bind()`.
- `AI_CANONNAME`: Usado para chamadas `getnameinfo()`.
- `AI_NUMERICHOST`: Impede a busca no DNS.
- `AI_NUMERICSERV`: Impede a busca no DNS.



# Estrutura `sockaddr_in`

A estrutura `sockaddr_in` é usada para armazenar informações de endereços IPv4 em sockets.

---

### Definição da Estrutura

```c
struct sockaddr_in {
    sa_family_t    sin_family; // Família de endereços (AF_INET)
    in_port_t      sin_port;   // Porta (em formato network byte order)
    struct in_addr sin_addr;   // Endereço IPv4
    char           sin_zero[8]; // Preenchimento para alinhar com `sockaddr`
};
```

---

### Campos

#### `sin_family`
- Especifica a família de endereços.
  - Sempre `AF_INET` para IPv4.

#### `sin_port`
- Porta de comunicação (16 bits).
  - Deve ser convertida para network byte order usando `htons()`.

#### `sin_addr`
- Endereço IPv4.
  - Contém um valor de tipo `struct in_addr`.
  - Representa o endereço IP em network byte order.

#### `sin_zero`
- Campo de preenchimento (8 bytes).
  - Deve ser preenchido com zeros (`memset()` ou inicialização explícita).

---

### Estrutura `in_addr`

Usada para armazenar um endereço IPv4.

```c
struct in_addr {
    uint32_t s_addr; // Endereço IPv4 em network byte order
};
```

---

### Exemplos de Uso

#### Configuração de um Endereço IPv4
```c
struct sockaddr_in server_addr;
memset(&server_addr, 0, sizeof(server_addr));

server_addr.sin_family = AF_INET;         // Família IPv4
server_addr.sin_port = htons(8080);      // Porta 8080 (convertida para network byte order)
server_addr.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer interface
```

---

### Funções Relacionadas

#### Conversão de Endereços
- `inet_pton()`: Converte um endereço IP em formato de string para `struct in_addr`.
  ```c
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
  ```

- `inet_ntop()`: Converte um endereço IP de `struct in_addr` para string.
  ```c
  char ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &server_addr.sin_addr, ip, INET_ADDRSTRLEN);
  ```

---

### Observações
- A estrutura `sockaddr_in` é frequentemente usada em conjunto com funções como `socket()`, `bind()`, `connect()` e `accept()`.
- É importante inicializar os campos corretamente para evitar comportamentos inesperados.

