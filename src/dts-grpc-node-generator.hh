#pragma once

#include <string>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include "dts-grpc-node-generator-options.hh"

class DtsGrpcNodeGenerator
  : public google::protobuf::compiler::CodeGenerator
{
public:

  bool PrintServiceImplementationInterface
    ( google::protobuf::io::Printer&              printer
    , const DtsGrpcNodeGeneratorOptions&          options
    , const google::protobuf::ServiceDescriptor*  service
    , std::string*                                error
    ) const;

  bool PrintServiceDefinitionInterface
    ( google::protobuf::io::Printer&              printer
    , const DtsGrpcNodeGeneratorOptions&          options
    , const google::protobuf::ServiceDescriptor*  service
    , std::string*                                error
    ) const;

  bool PrintServiceClientClass
    ( google::protobuf::io::Printer&              printer
    , const DtsGrpcNodeGeneratorOptions&          options
    , const google::protobuf::ServiceDescriptor*  service
    , std::string*                                error
    ) const;

  bool GenerateImports
    ( google::protobuf::io::Printer&           printer
    , const DtsGrpcNodeGeneratorOptions&       options
    , const google::protobuf::FileDescriptor*  file
    , std::string*                             error
    ) const;

  bool Generate
    ( const google::protobuf::FileDescriptor*        file
    , const std::string&                             parameter
    , google::protobuf::compiler::GeneratorContext*  context
    , std::string*                                   error
    ) const override;

};
