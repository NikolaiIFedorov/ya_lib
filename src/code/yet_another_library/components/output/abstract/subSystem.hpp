#pragma once

#include "log.hpp"
#include "portDesc.hpp"
#include <functional>

template <typename... MacroArgs> struct SubSystem {
    friend class PortDesc;

  public:
    using Init = std::function<void(SubSystem &)>;
    explicit SubSystem(Init init) {
        init(*this);
    };

    void operator()(MacroArgs... macroArgs) const {
        const auto &subMacros = this->subMacros;
        Log::Trace([subMacros, macroArgs...] {
            for (const auto &subMacro : subMacros)
                subMacro(macroArgs...);
        });
    };

    void addSubMacro(const PortDesc &port, std::function<float(MacroArgs...)> subMacro) {
        Log::Trace([this, port, subMacro] {
            subMacros.push_back(
                [port, subMacro](MacroArgs... macroArgs) { port(subMacro(macroArgs...)); });
        });
    };

  private:
    std::vector<std::function<void(MacroArgs...)>> subMacros;
};
