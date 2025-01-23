{
  "targets": [
    {
      "target_name": "manatee",
      "sources": [ "src/manatee.cpp" ],
      "cflags_cc!": [ "-Os", "-Wall", "-Wextra" ],
      "libraries": [ "-l:libBarcodeScanner.a" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "defines": [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }
  ]
}
