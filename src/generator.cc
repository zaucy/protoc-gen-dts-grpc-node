#include "dts-grpc-node-generator.hh"

int main(int argc, char* argv[]) {
  DtsGrpcNodeGenerator generator;
  PluginMain(argc, argv, &generator);
  return 0;
}
