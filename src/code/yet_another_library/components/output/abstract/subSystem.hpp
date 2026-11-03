#pragma once

#include "eventPort.hpp"
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

    void addSubMacro(const SystemPort &eventPort, std::function<float(MacroArgs...)> subMacro) {
        const auto &subMacros = this->subMacros;
        Log::Trace([subMacros, eventPort, subMacro] {
            subMacros.push_back([eventPort, subMacro](MacroArgs... macroArgs) {
                eventPort.write(subMacro(macroArgs...));
            });
        });
    };

  private:
    std::vector<std::function<void(MacroArgs...)>> subMacros;
};