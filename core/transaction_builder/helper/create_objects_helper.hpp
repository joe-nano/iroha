/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef CORE_TRANSACTION_BUILDER_CREATE_OBJECTS_HPP
#define CORE_TRANSACTION_BUILDER_CREATE_OBJECTS_HPP

#include <assert.h>
#include <algorithm>
#include <tuple>

#include <infra/protobuf/api.pb.h>
#include <util/exception.hpp>

namespace txbuilder {

/*
  Primitives
*/

static Api::BaseObject createValueString(std::string val) {
  Api::BaseObject ret;
  ret.set_valuestring(std::move(val));
  return ret;
}

static Api::BaseObject createValueInt(int val) {
  Api::BaseObject ret;
  ret.set_valueint(static_cast<::google::protobuf::int64>(val));
  return ret;
}

static Api::BaseObject createValueBool(bool val) {
  Api::BaseObject ret;
  ret.set_valueboolean(val);
  return ret;
}

static Api::BaseObject createValueDouble(double val) {
  Api::BaseObject ret;
  ret.set_valuedouble(val);
  return ret;
}

static Api::Trust createTrust(double value, bool isOk) {
  Api::Trust ret;
  ret.set_value(value);
  ret.set_isok(isOk);
  return ret;
}

/*
  Map
*/

using Map = std::map<std::string, Api::BaseObject>;

/*
  BaseObject
*/

const std::string PrefixString = "String_";
const std::string PrefixInteger = "Integer_";
const std::string PrefixBoolean = "Boolean_";
const std::string PrefixDouble = "Double_";

static std::string stringify(Api::BaseObject obj) {
  switch (obj.value_case()) {
  case Api::BaseObject::ValueCase::kValueString:
    return PrefixString + obj.valuestring();
  case Api::BaseObject::ValueCase::kValueInt:
    return std::to_string(obj.valueint());
  case Api::BaseObject::ValueCase::kValueBoolean:
    return obj.valueboolean() ? std::string("true") : "false";
  case Api::BaseObject::ValueCase::kValueDouble:
    return std::to_string(obj.valuedouble());
  default:
    throw "invalid type exception";
  }
}

namespace detail {

static std::tuple<std::string, std::string> splitPrefixValue(const std::string &s) {
  std::size_t mid = s.find("_");
  if (mid == std::string::npos)
    throw "Not found split mid point.";
  if (mid + 1 >= s.size())
    throw "Not found value";
  return std::make_tuple(s.substr(0, mid), s.substr(mid + 1));
}
}

static Api::BaseObject parse(std::string str) {
  Api::BaseObject ret;
  std::string prefix, value;
  std::tie(prefix, value) = detail::splitPrefixValue(str);
  if (prefix == PrefixString) {
    return createValueString(value);
  } else if (prefix == PrefixInteger) {
    for (auto c: value) {
      if (not isdigit(c)) throw "Value type mismatch";
    }
    return createValueInt(std::stoi(value));
  } else if (prefix == PrefixBoolean) {
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    return createValueBool(value == "true");
  } else if (prefix == PrefixDouble) {
    return createValueDouble(std::stod(value));
  } else {
    throw "Cannot parse value type from string.";
  }
}

static std::string stringify(::txbuilder::Map m) {
  std::string ret = "{";
  for (auto &&e : m) {
    ret += "(" + e.first + ", " + txbuilder::stringify(e.second) + "), ";
  }
  ret += "}";
  return ret;
}

/*
  Vector
*/
template <typename T> using Vector = ::google::protobuf::RepeatedPtrField<T>;

template <typename T>
inline std::vector<T>
createStandardVector(const ::txbuilder::Vector<T> &protov) {
  return std::vector<T>(protov.begin(), protov.end());
}

/*
  Assets
*/
static Api::Domain createDomain(std::string ownerPublicKey, std::string name) {
  Api::Domain ret;
  ret.set_ownerpublickey(std::move(ownerPublicKey));
  ret.set_name(std::move(name));
  return ret;
}

static Api::Account createAccount(std::string publicKey, std::string name,
                                  std::vector<std::string> assets) {
  Api::Account ret;
  ret.set_publickey(std::move(publicKey));
  ret.set_name(std::move(name));
  *ret.mutable_assets() = ::google::protobuf::RepeatedPtrField<std::string>(
      assets.begin(), assets.end());
  return ret;
}

static Api::Asset createAsset(std::string domain, std::string name,
                              ::txbuilder::Map value,
                              std::string smartContractName) {
  Api::Asset ret;
  ret.set_domain(std::move(domain));
  ret.set_name(std::move(name));
  *ret.mutable_value() = ::google::protobuf::Map<std::string, Api::BaseObject>(
      value.begin(), value.end());
  ret.set_smartcontractname(std::move(smartContractName));
  return ret;
}

static Api::SimpleAsset createSimpleAsset(std::string domain, std::string name,
                                          Api::BaseObject value,
                                          std::string smartContractName) {
  Api::SimpleAsset ret;
  ret.set_domain(std::move(domain));
  ret.set_name(std::move(name));
  *ret.mutable_value() = std::move(value);
  ret.set_smartcontractname(std::move(smartContractName));
  return ret;
}

static Api::Peer createPeer(std::string publicKey, std::string address,
                            Api::Trust trust) {
  Api::Peer ret;
  ret.set_publickey(std::move(publicKey));
  ret.set_address(std::move(address));
  *ret.mutable_trust() = std::move(trust);
  return ret;
}
}

#endif