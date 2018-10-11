cc_binary(
  name = "protoc-gen-dts-grpc-node",
  linkstatic = True,
  # features = ["fully_static_link"],
  srcs = [
    "src/generator.cc",
    "src/dts-grpc-node-generator-options.cc",
    "src/dts-grpc-node-generator-options.hh",
    "src/dts-grpc-node-generator.cc",
    "src/dts-grpc-node-generator.hh",
  ],
  deps = [
    "@com_google_protobuf//:protoc_lib",
  ],
)
