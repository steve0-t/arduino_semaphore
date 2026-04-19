{
  description = "Flake for C environment";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-25.05";
    nixpkgs-unstable.url = "nixpkgs/nixos-unstable";
  };

  outputs = {nixpkgs, ...}: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
    i2c_lib = pkgs.stdenv.mkDerivation {
      pname = "liquidcrystal-i2c";
      version = "1.1.2";

      src = pkgs.fetchFromGitHub {
        owner = "johnrickman";
        repo = "LiquidCrystal_I2C";
        rev = "1.1.2";
        hash = "sha256-TaMEgbFdDMq9l3xwlT7hBe/UShXCTYJy8TJFtUCcCP8=";
      };

      installPhase = ''
        mkdir -p $out/libraries/LiquidCrystal_I2C
        cp -r ./* $out/libraries/LiquidCrystal_I2C/
      '';
    };
  in {
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        meson
        valgrind
        pkg-config
        ninja
        clang-tools
        clang
        glibc
        gcc

        platformio
        platformio-core.udev
        unity-test
      ];

      shellHook = ''
      '';
    };
  };
}
