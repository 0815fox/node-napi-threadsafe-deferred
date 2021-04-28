{
  "targets": [
    {
      "target_name": "napi-threadsafe-deferred",
      "type": "static_library",
      "sources": [
        "src/ThreadSafeDeferred.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          "-std=c++11"
        ],
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
      },
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": [
        "NAPI_CPP_EXCEPTIONS"
      ]
    }
  ]
}
