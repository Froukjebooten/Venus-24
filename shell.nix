{ pkgs ? import <nixpkgs> {} }:


pkgs.mkShell {
  nativeBuildInputs = [
    pkgs.gtest.dev
    pkgs.gcc
    pkgs.gnumake
    pkgs.gcc-14
    pkgs.g++-14
  ];
  buildInputs = [
    pkgs.gtest
  ];
} 