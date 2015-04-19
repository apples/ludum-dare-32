//
// created by Patrick Brukiewa - 18 Apr 15
//

#include "Ai.hpp"

void update_ais(DB& db) {
    for (auto ent : db.query<AIComponent>()) {
        std::get<1>(ent).data().update(std::get<0>(ent));
    }
}
