# Maintainer: Ailton <ailton-0041>

pkgname=update-cachyos
pkgver=1.0
pkgrel=1
pkgdesc="Interface grafica simples para atualizar CachyOS e Flatpak"
arch=('x86_64')
url="https://github.com/ailton-0041/update-cachyOS"
license=('GPL3')
depends=('qt6-base' 'flatpak' 'polkit')
makedepends=('cmake' 'gcc')
source=("$pkgname-$pkgver.tar.gz::https://github.com/ailton-0041/update-cachyOS/archive/refs/tags/v$pkgver.tar.gz")
sha256sums=('SKIP')

package() {
    cd "$srcdir/update-cachyOS-$pkgver"

    cmake -B build -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build

    install -Dm755 build/AtualizadorCachyOS \
        "$pkgdir/usr/bin/update-cachyos"

    install -Dm644 resources/update-cachyos.desktop \
        "$pkgdir/usr/share/applications/update-cachyos.desktop"
}
