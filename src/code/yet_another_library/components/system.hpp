#pragma once

#include "eventPort.hpp"
#include <functional>

template <typename... MacroArgs> struct System {
    friend class PortInstance;

  public:
    using Init = std::function<void(System &)>;
    explicit System(Init init) {
        init(*this);
    };

    void operator()(MacroArgs... macroArgs) const {
        const auto &subMacros = this->subMacros;
        Log::Trace([subMacros, macroArgs...] {
            for (const auto &subMacro : subMacros)
                subMacro(macroArgs...);
        });
    };

    void addSubMacro(const EventPort &eventPort, std::function<float(MacroArgs...)> subMacro) {
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