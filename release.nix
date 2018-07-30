{ nixpkgs ? <nixpkgs>
, systems ? [ "i686-linux" "x86_64-linux" ]
, buildForAmiga ? false
, buildForWindows ? false
, amigaosenvPath ? <amigaosenv>
, kickstartROMFile ? null
, baseDiskImage ? null
, useUAE ? true
, libiffJobset ? import ../libiff/release.nix { inherit nixpkgs systems officialRelease buildForAmiga buildForWindows kickstartROMFile baseDiskImage useUAE; }
, libilbm ? {outPath = ./.; rev = 1234;}
, officialRelease ? false
}:

let
  pkgs = import nixpkgs {};
  
  version = builtins.readFile ./version;
  
  jobs = rec {
    tarball =
      with pkgs;

      let
        libiff = builtins.getAttr (builtins.currentSystem) (libiffJobset.build);
      in
      releaseTools.sourceTarball {
        name = "libilbm-tarball";
        src = libilbm;
        inherit version officialRelease;
        dontBuild = false;
        CFLAGS = "-ansi -pedantic -Wall";

        buildInputs = [ pkgconfig help2man libiff ];
      };
      
    build =
      (pkgs.lib.genAttrs systems (system:
        with import nixpkgs { inherit system; };
        
        let
          libiff = builtins.getAttr system (libiffJobset.build);
        in
        releaseTools.nixBuild {
          name = "libilbm";
          inherit version;
          src = tarball;
          buildInputs = [ pkgconfig libiff ];
          CFLAGS = "-ansi -pedantic -Wall";
        }
      )) //
      (pkgs.lib.optionalAttrs (buildForWindows) { i686-windows =
         let
           libiff = libiffJobset.build.i686-windows;
         in
         pkgs.dotnetenv.buildSolution {
           name = "libilbm";
           src = ./.;
           baseDir = "src";
           slnFile = "libilbm.sln";
           preBuild = ''
             export msBuildOpts="/p:libiffIncludePath=\"$(cygpath --windows ${libiff}/include)\" /p:libiffLibPath=\"$(cygpath --windows ${libiff})\""
           '';
           postInstall = ''
             mkdir -p $out/include/libilbm
             cp -v libilbm/*.h $out/include/libilbm
           '';
          };
        }) //
      (pkgs.lib.optionalAttrs (buildForAmiga)
        (let
          amigaosenv = import amigaosenvPath {
            inherit (pkgs) stdenv fetchurl lhasa uae fsuae procps bchunk cdrtools;
            inherit (pkgs.xorg) lndir;
          };
        in
        {
          m68k-amigaos.lib = let
            libiff = libiffJobset.build.m68k-amigaos.lib;
          in
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
            inherit kickstartROMFile baseDiskImage useUAE;
          };
        
          m68k-amigaos.tools = let
            libiff = libiffJobset.build.m68k-amigaos.lib;
          in
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
            inherit kickstartROMFile baseDiskImage useUAE;
          };
        }));
  };
in
jobs
