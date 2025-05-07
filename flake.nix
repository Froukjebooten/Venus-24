{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
  };

  outputs = { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = [
          pkgs.gcc14
          pkgs.gnumake
          pkgs.gtest.dev
        ];
        shellHook = ''
          export TEST_CFLAGS="-I${pkgs.gtest.dev}/include"
          export TEST_LDFLAGS="-L${pkgs.gtest}/lib"
        '';
      };
    };
}