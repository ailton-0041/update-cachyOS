#!/bin/bash
# Instalador do Atualizador CachyOS
# Uso: chmod +x install.sh && ./install.sh

set -e

echo "=============================="
echo " Atualizador CachyOS"
echo " Instalacao simples"
echo "=============================="
echo ""

# Verificar se esta rodando como root
if [ "$EUID" -eq 0 ]; then
    echo "AVISO: Nao execute como root! Execute como usuario normal."
    echo "O script usara 'sudo' quando necessario."
    exit 1
fi

# Descobrir diretorio do script
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR"

echo "[1/5] Instalando dependencias..."
sudo pacman -S --needed --noconfirm \
    qt6-base \
    flatpak \
    polkit \
    cmake \
    gcc \
    make \
    2>&1 | tail -1

echo "[2/5] Compilando..."
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

echo "[3/5] Instalando binario..."
sudo install -Dm755 build/AtualizadorCachyOS /usr/local/bin/update-cachyos

echo "[4/5] Instalando entrada no menu KDE..."
sudo install -Dm644 resources/update-cachyos.desktop \
    /usr/local/share/applications/update-cachyos.desktop

echo "[5/5] Atualizando cache do menu..."
sudo update-desktop-database /usr/local/share/applications 2>/dev/null || true

echo ""
echo "=============================="
echo " Instalacao concluida!"
echo "=============================="
echo ""
echo "Para executar:"
echo "  - Menu KDE > Atualizador CachyOS"
echo "  - Ou digite no terminal: update-cachyos"
echo ""
