#pragma once

enum RelayType {NORMALLY_CLOSED, NORMALLY_OPEN};

class RelayToggler {
    private:
        int relayPin;
        RelayType relayType;
        int getRelayPin();
    public:
        RelayToggler(int relayPinVal, RelayType relayTypeVal);
        void turnOn();
        void turnOff();
};