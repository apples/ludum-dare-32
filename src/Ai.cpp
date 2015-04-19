//
// created by Patrick Brukiewa - 18 Apr 15
//

#include "Ai.hpp"

int main() {
    GoombaAI gbrain;

    DB db;
    auto goomba_id = db.makeEntity();
    db.makeComponent(goomba_id, AIComponent(gbrain));

    for (auto ent : db.query<AIComponent>())
        std::get<1>(ent).data().update(std::get<0>(ent));

    return 0;


}
