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
          pkgs.entr
          (pkgs.writeScriptBin "watch_tests" ''
            #!${pkgs.bash}/bin/bash
            find . -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "Makefile" \) | entr -r bash -c '
              make test
              if [ $? -eq 0 ]; then
                echo -e "\n\033[1;32m╔════════════════════════════════════════╗"
                echo -e "║              TEST SUCCESS              ║"
                echo -e "╚════════════════════════════════════════╝\033[0m\n"
              else
                echo -e "\n\033[1;31m╔════════════════════════════════════════╗"
                echo -e "║              TEST FAILURE              ║"
                echo -e "╚════════════════════════════════════════╝\033[0m\n"
              fi
            '
          '')
        ];
        shellHook = ''
          export TEST_CFLAGS="-I${pkgs.gtest.dev}/include"
          export TEST_LDFLAGS="-L${pkgs.gtest}/lib"
        '';
      };
    };
}