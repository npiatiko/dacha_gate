#include <cstdint>
#include <atomic>
#include <mutex>
#include <condition_variable>

enum class State : uint8_t {
    CLOSED,
    OPENING,
    OPENED,
    CLOSING,
    ERRROR,
};

enum class MotorMode : uint8_t {
    FORWARD = 1u,
    REVERSE,
    OFF
};

union u_MotorMode {
    const MotorMode _mode;
    struct {
        const uint8_t _forward : 1;
        const uint8_t _reverse : 1;
    };
    u_MotorMode(const MotorMode mode) : _mode(mode) {}
};

class gateController {
   private:
    static constexpr int forwardPIN = 1;
    static constexpr int reversePIN = 0;
    static constexpr int motorBrakePIN = 2;
    static constexpr int irSensorPIN = 21;
    static constexpr int closeSensorPIN = 5;
    static constexpr int openSensorPIN = 27;

    static std::mutex mLock;
    static std::condition_variable mCv;

    static std::atomic<State> mState;

    gateController(const gateController&) = delete;
    gateController& operator=(gateController&) = delete;
    gateController();
    static void MotorControl(const MotorMode mode);

   public:
    static gateController& getController();
    static void IrSensorIntrpt();
    static void CloseSensorIntrpt();
    static void OpenSensorIntrpt();
    void openGate();
    void closeGate();

    ~gateController() = default;
};
