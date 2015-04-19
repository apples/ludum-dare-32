//
// created by Patrick Brukiewa - 18 Apr 15
//

#include "Ai.hpp"

void update_ais(Engine& engine, DB& db) {
    for (auto ent : db.query<AIComponent>()) {
        std::get<1>(ent).data().update(engine, std::get<0>(ent));
    }
}
