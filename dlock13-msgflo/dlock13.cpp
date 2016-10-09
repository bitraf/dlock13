#include <cstdlib>
#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <fstream>
#include <time.h>

#include "msgflo.h"

using namespace std;

// Minimalistic design-by-contracts support
// Throws Exception containing @msg if @predicate is not satisfied
#define PRECONDITION(predicate, msg) \
do { \
    if (!(predicate)) { \
        throw std::domain_error(std::string("Precondition failed: ") + msg); \
    } \
} while(0)

#define POSTCONDITION(predicate, msg) \
do { \
    if (!(predicate)) { \
        throw std::logic_error(std::string("Postcondition failed: ") + msg); \
    } \
} while(0)


class LockState {
public:
    bool isOpen = false;
    time_t openUntil = 0;
public:
    // actually an invariant of this data
    bool isValid() const {
        if (!isOpen) {
            return openUntil == 0; // always 0 if not open
        } else {
            return (openUntil > 1451606400); // can't be in the past
        }
    }

    bool operator ==(const LockState &b) const {
        return this->isOpen == b.isOpen && this->openUntil == b.openUntil;
    }
    bool operator !=(const LockState &b) const {
        return !(*this == b);
    }
};

LockState unlock(const LockState currentState, int openSeconds, time_t currentTime, int maxOpenSeconds) {
    // preconditions
    PRECONDITION(currentState.isValid(), "Current state is not valid");
    PRECONDITION(openSeconds > 0, "Time to open is negative or 0");
    PRECONDITION(currentTime > 1451606400, "Current time is before the code was made (2016)");
    PRECONDITION(maxOpenSeconds > 0, "Maximum time to open is negative or 0");
    PRECONDITION(maxOpenSeconds < 10*60, "Maximum time to open is extremely long");
    PRECONDITION(openSeconds <= maxOpenSeconds, "Time to open is longer than maximum");

    LockState newState;
    // FIXME: implement

    // postconditions
    POSTCONDITION(newState.isValid(), "New state is not valid");
    POSTCONDITION(newState.isOpen, "Lock did not open");
    POSTCONDITION(newState.openUntil > currentState.openUntil, "Lock is not open for longer than before");
    POSTCONDITION(newState.openUntil > currentState.openUntil+openSeconds, "Lock is not open for at least @openSeconds");

    return newState;
}

LockState tryLock(const LockState currentState, time_t currentTime, int maxOpenSeconds) {
    // preconditions    
    PRECONDITION(currentState.isValid(), "Current state is not valid");
    PRECONDITION(currentTime > 1451606400, "Current time is before the code was made (2016)");
    PRECONDITION(maxOpenSeconds > 0, "Maximum time to open is negative or 0");
    PRECONDITION(maxOpenSeconds < 10*60, "Maximum time to open is extremely long");
//    PRECONDITION(currentTime < currentState.+maxOpenSeconds, "Current time forwarded by more than maximum open time");

    LockState newState;
    // FIXME: implement

    // postconditions
    POSTCONDITION(newState.isValid(), "New state is not valid");

    return newState;
}

static const int MAX_UNLOCK_SECONDS = 2*60;

class DoorLock {

public:
    DoorLock(string gpio)
        : gpio_path(gpio)
    {
    }

public:
    void setParticipant(msgflo::Participant *part) {
        participant = part;
    }

    void check() {
        try {
            const time_t currentTime = time(NULL);
            const LockState newState = tryLock(state, currentTime, MAX_UNLOCK_SECONDS);
            if (newState != state) {
                POSTCONDITION(!state.isOpen, "Lock open after successful locking");
                setState(newState);
            }
        } catch (const std::exception &e) {
            cerr << e.what() << endl;
            setState(LockState());  // Try to ensure gets locked anyway
            send("error", std::string(e.what()));
        }
    }

    virtual void process(string port, msgflo::Message *msg) {
        try {
            PRECONDITION(port == "open", "Invalid inport name");
            const int openSeconds = std::stoi(msg->asString());
            cout << "DoorLock.open(" << openSeconds << ")" << endl;
            const time_t currentTime = time(NULL);
            const LockState newState = unlock(state, openSeconds, currentTime, MAX_UNLOCK_SECONDS);
            POSTCONDITION(state.isOpen, "Lock not open after successful unlocking");
            setState(state);
        } catch (const std::exception& e) {
            // Don't change state, just notify of the problem
            cerr << e.what() << endl;
            send("error", std::string(e.what()));
        }

        // always ACK, even errors are considered "done processing"
        msg->ack();
    }

private:
    void send(std::string port, const std::string &data) {
        participant->send(port, data);
    }

    void setGPIO(const std::string &filepath, std::string value) {
        PRECONDITION(filepath.length(), "Filepath is empty");
        std::ofstream fs(filepath.c_str());
        PRECONDITION(fs, "Filepath cannot be opened");

        cout << "DoorLock set GPIO: " << filepath << "to" << value << endl;
        fs << value;
        fs.close();

        POSTCONDITION(fs.is_open(), "File stream was not closed");
        // POSTCONDITION: file now contains @value
    }

    void setState(LockState s) {
        PRECONDITION(s.isValid(), "New state not valid");

        // Actualize the state
        state = s;
        setGPIO(gpio_path, state.isOpen ? "1" : "0");

        // Notify about change
        send("isopen", std::string(state.isOpen ? "true" : "false"));
        const std::string openUntil = std::to_string(state.openUntil);
        send("openuntil", openUntil);
        cout << "DoorLock open until " << openUntil << endl;

        // POSTCONDITION: door solenoid now reflects new state
    }

private:
    LockState state;
    std::string gpio_path;
    msgflo::Participant *participant;
};

atomic_bool run(true);

int main(int argc, char **argv) {

    std::string role = "dlock13-1";
    if (argc > 1) {
        role = argv[1];
    }
    std::string file = "/sys/class/gpio/gpio11/value";
    if (argc > 2) {
        file = argv[2];
    }

    msgflo::Definition def;
    def.component = "Bitraf Dlock13";
    def.label = "Door lock";
    def.inports = {
        {"open", "int", "Open the door for N seconds (maximum 120)"},
    };
    def.outports = {
        {"isopen", "boolean", "Whether the door is open or not"},
        {"openuntil", "int", "What time the door will be open until (Unix timestamp)"},
        {"error", "string", "Error from trying to open door"},
    };

    msgflo::EngineConfig config;
    auto engine = msgflo::createEngine(config);

    DoorLock dlock(file);
    msgflo::Participant *participant = engine->registerParticipant(def, [&](msgflo::Message *msg) {
        // TODO: make msgflo-cpp support port name in processing function
        dlock.process("open", msg);
    });
    dlock.setParticipant(participant); // TODO: avoid DoorLock needing to know Participant?

    while(run) {
        this_thread::sleep_for(chrono::seconds(10));
        dlock.check();
    }
    // FIXME: run this while still calling .check() periodically engine->launch();

    return EXIT_SUCCESS;
}
