<div align="center">

# ⚡ Atualizador CachyOS

**Interface gráfica simples para manter seu CachyOS e Flatpak sempre atualizados**

![License](https://img.shields.io/badge/licença-MIT-blue)
![Plataforma](https://img.shields.io/badge/plataforma-CachyOS%20%7C%20Arch%20Linux-brightgreen)
![Feito com](https://img.shields.io/badge/feito%20com-C%2B%2B%20%7C%20Qt6-orange)

---

</div>

## Índice

- [Sobre](#sobre)
- [Funcionalidades](#funcionalidades)
- [Captura de Tela](#captura-de-tela)
- [Instalação](#instalação)
  - [Método 1 — Script Automático (recomendado)](#método-1--script-automático-recomendado)
  - [Método 2 — PKGBUILD](#método-2--pkgbuild)
  - [Método 3 — Manual](#método-3--manual)
- [Como Usar](#como-usar)
- [Compilar do Zero](#compilar-do-zero)
- [Licença](#licença)

---

## Sobre

O **Atualizador CachyOS** é um programa com interface gráfica feita para o KDE que permite atualizar o sistema CachyOS e os aplicativos Flatpak com apenas um clique.

Feito para **iniciantes** e **usuários avançados** que preferem uma interface visual ao invés do terminal.

> Código 100% livre e aberto. Sinta-se à vontade para modificar, estudar e compartilhar.

---

## Funcionalidades

- **Verificação automática** ao abrir o programa
- **Lista organizada** com nome do pacote, versão atual e nova versão
- **Atualização completa** do sistema (`pacman -Syu`)
- **Atualização de Flatpaks** (`flatpak update`)
- **Botão único** "Atualizar Tudo" para fazer ambos de uma vez
- **Interface nativa**, combina perfeitamente com o KDE Plasma
- **Leve e rápido** — compilado em C++ com Qt6
- **Re-verificação automática** após concluir as atualizações

---

## Captura de Tela

<div align="center">

*(Adicione aqui uma imagem da interface)*

![Screenshot do programa](screenshot.png)

</div>

---

## Instalação

### Método 1 — Script Automático (recomendado)

Para usuários iniciantes, o jeito mais fácil:

```bash
git clone https://github.com/ailton-0041/update-cachyOS.git
cd update-cachyOS
chmod +x install.sh
./install.sh
```

O script instala todas as dependências, compila e configura o programa no sistema automaticamente.

### Método 2 — PKGBUILD

Para usuários que preferem o modo Arch Linux nativo:

```bash
git clone https://github.com/ailton-0041/update-cachyOS.git
cd update-cachyOS
makepkg -si
```

### Método 3 — Manual

```bash
# Dependências
sudo pacman -S --needed qt6-base flatpak polkit cmake gcc make

# Compilar
git clone https://github.com/ailton-0041/update-cachyOS.git
cd update-cachyOS
mkdir -p build && cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Instalar
sudo install -Dm755 build/AtualizadorCachyOS /usr/local/bin/update-cachyos
sudo install -Dm644 resources/update-cachyos.desktop /usr/local/share/applications/
sudo update-desktop-database /usr/local/share/applications
```

Após a instalação, o programa aparece no **Menu KDE** como **Atualizador CachyOS**.

---

## Como Usar

1. Abra o programa pelo menu KDE ou execute `update-cachyos` no terminal
2. Ele já vai procurar atualizações automaticamente
3. Se houver atualizações, clique em **Atualizar Tudo**
4. Digite sua senha (solicitação do `pkexec`)
5. Pronto! O sistema será re-verificado após a conclusão

---

## Compilar do Zero

Se quiser compilar manualmente sem instalar:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/AtualizadorCachyOS
```

**Requisitos:**
- Compilador C++ (gcc ou clang)
- CMake ≥ 3.16
- Qt6 (Widgets)
- Flatpak (para detectar e atualizar Flatpaks)
- Polkit (pkexec) — Pré-instalado no CachyOS

---

## Licença

Este projeto é **código livre** — distribuído sob a licença **MIT**.

Você pode usar, copiar, modificar, mesclar, publicar, distribuir e até vender cópias do software, desde que mantenha o aviso de direitos autorais.

---

<div align="center">

Criado por [Ailton](https://github.com/ailton-0041) &nbsp;|&nbsp; [Atualizador CachyOS](https://github.com/ailton-0041/update-cachyOS)

</div>
