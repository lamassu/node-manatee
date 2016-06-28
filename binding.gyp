{
  "targets": [
    {
      "target_name": "manatee",
      "sources": [ "src/manatee.cc" ],
      "libraries": [ "-lBarcodeScanner" ],
      "include_dirs" : ["./node_modules/nan"]
    }
  ]
}
