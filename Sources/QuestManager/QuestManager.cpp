#include "QuestManager.hpp"

#include "ItemDatabase.hpp"

void QuestManager::loadQuests() {
    // --- [Begin] - Explore strange path --- 
    quests.push_back(Quest(0, "Explore strange paths", 40));
    quests.back().addRequiredLevel(1);
    quests.back().addRequiredDescription("Talk to Elder Thorne. Min Lv.1");
    // Stage 0: Elder Thorne cảnh báo
    quests.back().addNpcID      (0, 0);
    quests.back().addDialogue   (0, "[1/4] Something stirs in the forest... It no longer feels safe");
    quests.back().addDialogue   (0, "[2/4] Long ago, we sealed away a dark presence beyond the hills");
    quests.back().addDialogue   (0, "[3/4] But now... the whispers return, and creatures creep closer each night");
    quests.back().addDialogue   (0, "[4/4] You must go, uncover the truth. Our hope rests with you");
    quests.back().addDescription(0, "Investigate the road beyond the wooden bridge");
    quests.back().addObjective  (0, std::make_shared<ExploreObjective>(1));
    // Stage 1: Quay về Elder Thorne
    quests.back().addNpcID      (1, 0);
    quests.back().addDialogue   (1, "[1/3] You're back... I feared the worst");
    quests.back().addDialogue   (1, "[2/3] This place was once peaceful. What have we awakened?");
    quests.back().addDialogue   (1, "[3/3] Find Torren. He'll help you prepare for what lies ahead");
    quests.back().addDescription(1, "Return to Elder Thorne");
    quests.back().addObjective  (1, std::make_shared<TalkObjective>(0));
    // Stage 2: Gặp Torren, nhận cung
    quests.back().addNpcID      (2, 1);
    quests.back().addDialogue   (2, "[1/7] Elder Thorne sent you? Then it must be serious");
    quests.back().addDialogue   (2, "[2/7] You're really heading out there? It's dangerous");
    quests.back().addDialogue   (2, "[3/7] I've seen strange tracks by the river. They weren't left by any animal");
    quests.back().addDialogue   (2, "[4/7] If you're going, take this bow. You'll need it");
    quests.back().addDialogue   (2, "[5/7] Press [E] to open your inventory, then drag the bow into your weapon slot");
    quests.back().addDialogue   (2, "[6/7] Press [Space] to use your bow. Stay sharp");
    quests.back().addDialogue   (2, "[7/7] From now on, you fight as an archer. Help us drive back the darkness");
    quests.back().addDescription(2, "Speak to Torren");
    quests.back().addObjective  (2, std::make_shared<TalkObjective>(1));
    quests.back().addItemFromNpc(2, ItemDatabase::get("Old Bow"));
    // Stage 3: Diệt quái
    quests.back().addNpcID      (3, -1);
    quests.back().addDialogue   (3, std::string());
    quests.back().addDescription(3, "Help the villages defeat the monsters");
    quests.back().addObjective  (3, std::make_shared<KillMonsterObjective>("Bat Lv.1", 10));
    quests.back().addObjective  (3, std::make_shared<KillMonsterObjective>("Bat Lv.3", 5));
    // Stage 4: Trở lại Torren
    quests.back().addNpcID      (4, 1);
    quests.back().addDialogue   (4, "[1/3] Back already? I see you've made it through");
    quests.back().addDialogue   (4, "[2/3] That bow served you well, huh? Told you it'd come in handy");
    quests.back().addDialogue   (4, "[3/3] You're stronger now. Maybe ready to carve your own legend");
    quests.back().addDescription(4, "Return to Torren");
    quests.back().addObjective  (4, std::make_shared<TalkObjective>(1));
    // --- [End] ---
    
    // --- [Begin] - Into the Darkwood ---
    quests.push_back(Quest(1, "Into the Darkwood", 200));
    quests.back().addRequiredLevel(3);
    quests.back().addRequiredQuestID(0);
    quests.back().addRequiredDescription("Talk to Mira. Min Lv.3");
    // Stage 0: Mira khởi đầu
    quests.back().addNpcID      (0, 2);
    quests.back().addDialogue   (0, "[1/4] I've heard strange noises deeper in the forest lately...");
    quests.back().addDialogue   (0, "[2/4] Something's not right. The animals are restless, the wind feels heavy");
    quests.back().addDialogue   (0, "[3/4] I think it's time you meet someone who knows more-Bren, the forest keeper");
    quests.back().addDialogue   (0, "[4/4] You'll find him beyond the old stone gate, deeper in the woods");
    quests.back().addDescription(0, "Head deeper into the forest and find Bren");
    quests.back().addObjective  (0, std::make_shared<TalkObjective>(2));
    // Stage 1: Gặp Bren
    quests.back().addNpcID      (1, 3);
    quests.back().addDialogue   (1, "[1/5] So Mira sent you... Then the forest truly is crying out");
    quests.back().addDialogue   (1, "[2/5] Long ago, a cursed gate was opened, unleashing horrors from beyond");
    quests.back().addDialogue   (1, "[3/5] We sealed the gate, but remnants remain—twisted creatures, each guarding a fragment of power");
    quests.back().addDialogue   (1, "[4/5] These fragments, we call them orbs. One in particular: the Bat Orb");
    quests.back().addDialogue   (1, "[5/5] Bring them to me. Only then can we confront what still lingers");
    quests.back().addDescription(1, "Head deeper into the forest and find Bren");
    quests.back().addObjective  (1, std::make_shared<TalkObjective>(3));
    // Stage 2: Kiếm đồ
    quests.back().addNpcID      (2, 3);
    quests.back().addDialogue   (2, std::string());
    quests.back().addDescription(2, "Collect the Bat Orb from corrupted creatures");
    quests.back().addObjective  (2, std::make_shared<CollectItemObjective>("Bat Orb", 1));
    // Stage 3: Trả đồ, nghe kể chuyện
    quests.back().addNpcID      (3, 3);
    quests.back().addDialogue   (3, "[1/6] You've returned with the orb... I feared they were lost");
    quests.back().addDialogue   (3, "[2/6] Each orb once belonged to ancient guardians-fallen to corruption");
    quests.back().addDialogue   (3, "[3/6] These guardians were sealed, but now their power stirs once more");
    quests.back().addDialogue   (3, "[4/6] We must awaken one-summon it at the altar, and destroy it completely");
    quests.back().addDialogue   (3, "[5/6] Only by cleansing the past can we close the cursed gate forever");
    quests.back().addDialogue   (3, "[6/6] Go. Use the orbs at the boss altar. Return to me once the deed is done");
    quests.back().addDescription(3, "Return to Bren with the orbs and listen to his tale");
    quests.back().addObjective  (3, std::make_shared<TalkObjective>(3));
    // Stage 4: Triệu hồi và tiêu diệt boss
    quests.back().addNpcID      (4, -1); // -1 nếu không cần NPC trong bước này
    quests.back().addDialogue   (4, std::string());
    quests.back().addDescription(4, "Find the altar to summon the boss and defeat it");
    quests.back().addObjective  (4, std::make_shared<KillMonsterObjective>("Bat Boss Lv.10", 1));
    // Stage 5: Gặp lại Bren
    quests.back().addNpcID      (5, 3); 
    quests.back().addDialogue   (5, "[1/4] So it's done... I felt the forest exhale for the first time in years");
    quests.back().addDialogue   (5, "[2/4] That beast was only one of many, sealed across the lands");
    quests.back().addDialogue   (5, "[3/4] The cursed gate is weakening still. To truly close it, more must fall");
    quests.back().addDialogue   (5, "[4/4] Rest for now. But when you're ready... there is more to be done");
    quests.back().addDescription(5, "Return to Bren after defeating the boss");
    quests.back().addObjective  (5, std::make_shared<TalkObjective>(3));
    // --- [End] ---

    // --- [Begin] - The Secret of the Gloom-Swamp ---
    quests.push_back(Quest(2, "The Secret of the Gloom-Swamp", 1000));
    quests.back().addRequiredLevel(6);
    quests.back().addRequiredQuestID(1); // Yêu cầu hoàn thành quest "Into the Darkwood"
    quests.back().addRequiredDescription("Speak to Bren again. Min Lv.8");

    // // Stage 0: Bren chỉ đường
    quests.back().addNpcID      (0, 3); // NPC Bren
    quests.back().addDialogue   (0, "[1/4] The forest breathes easier, but the corruption's roots run deep");
    quests.back().addDialogue   (0, "[2/4] The spirit you cleansed was of the sky");
    quests.back().addDialogue   (0, "[3/8] Its counterpart, a guardian of the deep places,");
    quests.back().addDialogue   (0, "[4/8] ...remains bound in the Gloom-Swamp to the southeast");
    quests.back().addDialogue   (0, "[5/8] Within that poisoned marsh lives an old hermit, Elara");
    quests.back().addDialogue   (0, "[6/8] She watches over the swamp's heart.");
    quests.back().addDialogue   (0, "[7/8] Find her. She will guide you on how to draw out the fallen guardian");
    quests.back().addDialogue   (0, "[8/8] Be wary of the fog and what lurks beneath the water.");
    quests.back().addDescription(0, "Travel southeast past the Darkwood to find the Gloom-Swamp");
    quests.back().addObjective  (0, std::make_shared<ExploreObjective>(5)); // ID khu vực đầm lầy là 5

    // // Stage 1: Gặp Elara
    quests.back().addNpcID      (1, 4); // NPC mới Elara, ID 4
    quests.back().addDialogue   (1, "[1/7] A new face... It has been a long time since anyone but the lost souls wandered here");
    quests.back().addDialogue   (1, "[2/7] Bren sent you? Then you carry a heavy purpose");
    quests.back().addDialogue   (1, "[3/7] You seek to quiet the Gazer Lord, the eye that never sleeps");
    quests.back().addDialogue   (1, "[4/7] Its corruption has blinded this land, turning clear waters into this murky poison");
    quests.back().addDialogue   (1, "[5/7]The creatures here weep corrupted tears");
    quests.back().addDialogue   (1, "[6/7] To summon it, you must gather these 'Eye Orb' from the most tormented spirits of the swamp.");
    quests.back().addDialogue   (1, "[7/7] Bring me three tears, and I will help you craft the lure to call it from the abyss.");
    quests.back().addDescription(1, "Speak to Elara in the heart of the Gloom-Swamp");
    quests.back().addObjective  (1, std::make_shared<TalkObjective>(4));

    // // Stage 2: Thu thập Nước Mắt Tuyệt Vọng
    quests.back().addNpcID      (2, 4);
    quests.back().addDialogue   (2, std::string());
    quests.back().addDescription(2, "Collect 'Eye Orb' from the swamp's creatures.");
    quests.back().addObjective  (2, std::make_shared<CollectItemObjective>("Eye Orb", 1)); // Item mới

    // // Stage 3: Quay lại Elara
    quests.back().addNpcID      (3, 4); 
    quests.back().addDialogue   (3, "[1/4] You have them... These poor souls. Their sorrow is potent.");
    quests.back().addDialogue   (3, "[2/4] I will combine their essence with the Bat Orb you carry. This will create a beacon.");
    quests.back().addDialogue   (3, "[3/4] Take that Orb to the stone altar nearby. Place it upon the pedestal.");
    quests.back().addDialogue   (3, "[4/4] The Gazer Lord will be drawn to the sorrow. End its suffering, hero. For all our sakes.");
    quests.back().addDescription(3, "Return to Elara with the Eye Orb.");
    quests.back().addObjective  (3, std::make_shared<TalkObjective>(4));

    // // Stage 4: Triệu hồi và tiêu diệt Gazer Lord
    quests.back().addNpcID      (4, -1);
    quests.back().addDialogue   (4, std::string());
    quests.back().addDescription(4, "Use the swamp altar to summon and defeat the Gazer Lord.");
    quests.back().addObjective  (4, std::make_shared<KillMonsterObjective>("Bat Boss Lv.10", 1)); // Boss mới

    // // Stage 5: Báo cáo với Elara
    quests.back().addNpcID      (5, 4); // NPC Elara
    quests.back().addDialogue   (5, "[1/5] The fog... it is lifting. The water seems... clearer. You have done it.");
    quests.back().addDialogue   (5, "[2/5] You have freed two great spirits now. Their combined energy reveals a path forward.");
    quests.back().addDialogue   (5, "[3/5] The source of the corruption, the Cursed Gate itself,");
    quests.back().addDialogue   (5, "[4/5] ...is guarded by the one who opened it-The Gatekeeper.");
    quests.back().addDialogue   (5, "[5/5] Go back to Bren. Tell him what we have learned. The final battle is drawing near.");
    quests.back().addDescription(5, "Return to Elara after defeating the Gazer Lord.");
    quests.back().addObjective  (5, std::make_shared<TalkObjective>(4));
    // --- [End] ---

    // quests.push_back(Quest(<name>, <exp>));
    // quests.back().addRequiredLevel(<playerLevel>);
    // quests.back().addRequiredDescription(<description>);

    // quests.back().addNpcID      (<stage>, <npcID>);
    // quests.back().addDialogue   (<stage>, <dialogue>);
    // quests.back().addDescription(<stage>, <description>);
    // quests.back().addObjective  (<stage>, <objective>);
}

std::vector<Quest>& QuestManager::getQuests() {
    return quests;
}