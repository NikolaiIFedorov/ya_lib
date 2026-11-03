#pragma once

#include "log.hpp"
#include "port.hpp"
#include <functional>

template <typename... MacroArgs> struct SubSystem {
    friend class PortInstance;

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

    void addSubMacro(const PortInstance &port, std::function<float(MacroArgs...)> subMacro) {
        const auto &subMacros = this->subMacros;
        Log::Trace([subMacros, port, subMacro] {
            subMacros.push_back(
                [port, subMacro](MacroArgs... macroArgs) { port(subMacro(macroArgs...)); });
        });
    };

  private:
    std::vector<std::function<void(MacroArgs...)>> subMacros;
};