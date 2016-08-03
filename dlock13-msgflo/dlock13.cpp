#include <cstdlib>
#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>

#include "msgflo.h"

using namespace std;

class DlockParticipant : public msgflo::Participant {
    struct Def : public msgflo::Definition {
        Def(void) : msgflo::Definition() {
            component = "Bitrad Dlock13";
            label = "Door lock";
            inports = {
                {"in", "any", ""}
            };
            outports = {
                {"out", "any", ""}
            };
        }
    };

public:
    DlockParticipant(string role) : msgflo::Participant(role, Def()) {
    }

private:
    virtual void process(string port, msgflo::Message msg) {
        cout << "Repeat.process()" << endl;
        msgflo::Message out;
        out.json = msg.json;
        send("out", out);
        ack(msg);
    }
};

atomic_bool run(true);

int main(int argc, char **argv) {

    DlockParticipant dlock("boxy-msgflo-2");

    msgflo::EngineConfig config;
    config.url("mqtt://mqtt.bitraf.no");
    config.participant(&dlock);
    auto engine = msgflo::createEngine(config);

    while(run) {
        this_thread::sleep_for(chrono::seconds(10));
    }

    return EXIT_SUCCESS;
}
