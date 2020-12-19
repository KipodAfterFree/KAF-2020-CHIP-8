#include "JITSection.h"

JITSection::JITSection(word startingAddr, word numInsns, asmjit::CodeHolder *code, bool shouldLog)
        : _startingAddr(startingAddr), _numInsns(numInsns), _logger(stdout), assm(code)
{
    for (int i = 0; i < numInsns; ++i)
    {
        _labels.push_back(assm.newLabel());
    }

    if (shouldLog)
    {
        assm.setLogger(&_logger);
    }
}

std::optional<asmjit::Label> JITSection::getLabelForAddress(word addr)
{
    if (addr & 1u) throw std::runtime_error("Odd address");
    if ((addr < _startingAddr) || (addr >= (_startingAddr + _numInsns * sizeof(opcode)))) return std::nullopt;
    return _labels.at((addr - _startingAddr) / 2);
}