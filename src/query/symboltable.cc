/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * Licensed under the MIT license (see LICENSE).
 */
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <unordered_map>
#include "symboltable.h"

namespace fnordmetric {
namespace query {

static std::unordered_map<std::string, const SymbolTableEntry*>*
  __globalSymbolTable() {
  static std::unordered_map<std::string, const SymbolTableEntry*> symbols;
  return &symbols;
}


const SymbolTableEntry* lookupSymbol(const std::string& symbol) {
  auto iter = __globalSymbolTable()->find(symbol);

  if (iter == __globalSymbolTable()->end()) {
    return nullptr;
  } else {
    return iter->second;
  }
}

SymbolTableEntry::SymbolTableEntry(
    const std::string& symbol,
    void (*method)(void*, int, SValue*, SValue*),
    size_t scratchpad_size) :
    call_(method),
    scratchpad_size_(scratchpad_size) {
  assert(__globalSymbolTable()->find(symbol) == __globalSymbolTable()->end());
  (*__globalSymbolTable())[symbol] = this;
}

SymbolTableEntry::SymbolTableEntry(
    const std::string& symbol,
    void (*method)(void*, int, SValue*, SValue*)) :
    SymbolTableEntry(symbol, method, 0) {}

bool SymbolTableEntry::isAggregate() const {
  return scratchpad_size_ > 0;
}

void (*SymbolTableEntry::getFnPtr() const)(void*, int, SValue*, SValue*) {
  return call_;
}

size_t SymbolTableEntry::getScratchpadSize() const {
  return scratchpad_size_;
}

}
}