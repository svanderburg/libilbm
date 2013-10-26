{ nixpkgs ? <nixpkgs>
, systems ? [ "i686-linux" "x86_64-linux" ]
, buildForAmiga ? false
, amigaosenvPath ? <amigaosenv>
}:

let
  pkgs = import nixpkgs {};
  
  version = builtins.readFile ./version;
  
  jobs = rec {
    tarball =
      { libilbm ? {outPath = ./.; rev = 1234;}
      , officialRelease ? false
      , libiff ? builtins.getAttr (builtins.currentSystem) ((import ../libiff/release.nix {}).build {})
      }:

      with pkgs;

      releaseTools.sourceTarball {
        name = "libilbm-tarball";
        src = libilbm;
        inherit version officialRelease;

        buildInputs = [ pkgconfig libiff ];
      };
      
    build =
      { tarball ? jobs.tarball {} }:
      
      (pkgs.lib.genAttrs systems (system:
        { libiff ? builtins.getAttr system ((import ../libiff/release.nix {}).build {}) }:
        
        with import nixpkgs { inherit system; };
        
        releaseTools.nixBuild {
          name = "libilbm";
          inherit version;
          src = tarball;
          buildInputs = [ pkgconfig libiff ];
        }
      )) //
      (pkgs.lib.optionalAttrs (buildForAmiga)
        (let
          amigaosenv = import amigaosenvPath {
            inherit (pkgs) stdenv uae procps;
            lndir = pkgs.xorg.lndir;
          };
        in
        {
          m68k-amigaos.lib = { libiff ? ((import ../libiff/release.nix { buildForAmiga = true; }).build {}).m68k-amigaos.lib }:
          
          amigaosenv.mkDerivation {
            name = "libilbm-${version}";
            src = "${tarball}/tarballs/libilbm-${version}pre1234.tar.gz";
      
            buildCommand = ''
              tar xfvz $src
              cd libilbm-${version}pre1234
              CFLAGS='-noixemul -O3' LIBIFF_CFLAGS='-I/gg/include' LIBIFF_LIBS='-L/gg/lib -liff' ./configure --prefix=/OUT --disable-shared
              cd src/libilbm
              make
              make install
            '';
            
            buildInputs = [ libiff ];
          };
        
          m68k-amigaos.tools = { libiff ? ((import ../libiff/release.nix { buildForAmiga = true; }).build {}).m68k-amigaos.lib }:
          
          amigaosenv.mkDerivation {
            name = "libilbm-${version}";
            src = "${tarball}/tarballs/libilbm-${version}pre1234.tar.gz";
      
            buildCommand = ''
              tar xfvz $src
              cd libilbm-${version}pre1234
              LIBIFF_CFLAGS='-I/gg/include' LIBIFF_LIBS='-L/gg/lib -liff' ./configure --prefix=/OUT --disable-shared
              make
              make check
              make install
            '';
            
            buildInputs = [ libiff ];
          };
        }));
  };
in
jobs
