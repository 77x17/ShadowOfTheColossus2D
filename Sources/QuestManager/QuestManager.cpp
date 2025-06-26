#include "QuestManager.hpp"

#include "ItemDatabase.hpp"

void QuestManager::loadQuests() {
    // --- [Begin] - [0] - Explore strange path --- 
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
    quests.back().addObjective  (3, std::make_shared<KillMonsterObjective>("Vampire Bat Lv.3", 10));
    // Stage 4: Trở lại Torren
    quests.back().addNpcID      (4, 1);
    quests.back().addDialogue   (4, "[1/3] Back already? I see you've made it through");
    quests.back().addDialogue   (4, "[2/3] That bow served you well, huh? Told you it'd come in handy");
    quests.back().addDialogue   (4, "[3/3] You're stronger now. Maybe ready to carve your own legend");
    quests.back().addDescription(4, "Return to Torren");
    quests.back().addObjective  (4, std::make_shared<TalkObjective>(1));
    // --- [End] ---
    
    // --- [Begin] - [1] - Into the Darkwood ---
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
    quests.back().addNpcID      (2, -1);
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
    quests.back().addObjective  (4, std::make_shared<KillMonsterObjective>("Bat Boss Lv.5", 1));
    // Stage 5: Gặp lại Bren
    quests.back().addNpcID      (5, 3); 
    quests.back().addDialogue   (5, "[1/4] So it's done... I felt the forest exhale for the first time in years");
    quests.back().addDialogue   (5, "[2/4] That beast was only one of many, sealed across the lands");
    quests.back().addDialogue   (5, "[3/4] The cursed gate is weakening still. To truly close it, more must fall");
    quests.back().addDialogue   (5, "[4/4] Rest for now. But when you're ready... there is more to be done");
    quests.back().addDescription(5, "Return to Bren after defeating the boss");
    quests.back().addObjective  (5, std::make_shared<TalkObjective>(3));
    // --- [End] ---

    // --- [Begin] - [2] - A Message for Elara ---
    quests.push_back(Quest(2, "A Message for Elara", 750)); 
    quests.back().addRequiredLevel(6);
    quests.back().addRequiredQuestID(1); // Yêu cầu hoàn thành quest "Into the Darkwood"
    quests.back().addRequiredDescription("Speak to Bren again. Min Lv.6");

    // Stage 0: Nhận nhiệm vụ từ Bren
    quests.back().addNpcID      (0, 3); // NPC Bren
    quests.back().addDialogue   (0, "[1/5] The corruption's heart lies deep within the Gloom-Swamp.");
    quests.back().addDialogue   (0, "[2/5] An old friend of mine, the hermit Elara, resides there.");
    quests.back().addDialogue   (0, "[3/5] She understands the land's secrets, but she is very cautious.");
    quests.back().addDialogue   (0, "[4/5] Find her. Tell her that Bren sent you, and the time has come.");
    quests.back().addDialogue   (0, "[5/5] She will need to trust you before she reveals anything more.");
    quests.back().addDescription(0, "Speak to Bren");
    quests.back().addObjective  (0, std::make_shared<TalkObjective>(3));

    // Stage 1: Đi đến đầm lầy
    quests.back().addNpcID      (1, -1);
    quests.back().addDialogue   (1, std::string());
    quests.back().addDescription(1, "Travel southeast past the Darkwood to find the Gloom-Swamp.");
    quests.back().addObjective  (1, std::make_shared<ExploreObjective>(5)); // ID khu vực đầm lầy là 5

    // Stage 2: Gặp Elara và nhận thử thách
    quests.back().addNpcID      (2, 4); // NPC Elara
    quests.back().addDialogue   (2, "[1/5] Who are you to disturb this dying place? Speak quickly.");
    quests.back().addDialogue   (2, "[2/5] Bren sent you? He always was too trusting of wanderers.");
    quests.back().addDialogue   (2, "[3/5] Words are wind. This swamp demands proof, not promises.");
    quests.back().addDialogue   (2, "[4/5] The very air here is poison. The creatures weep with corruption.");
    quests.back().addDialogue   (2, "[5/5] Show me you can face this blight. Let your actions speak for you.");
    quests.back().addDescription(2, "Find and speak to Elara in the heart of the Gloom-Swamp.");
    quests.back().addObjective  (2, std::make_shared<TalkObjective>(4));

    // Stage 3: Tiêu diệt quái vật để chứng tỏ sức mạnh
    quests.back().addNpcID      (3, -1);
    quests.back().addDialogue   (3, std::string());
    quests.back().addDescription(3, "Defeat 10 Eye Lv.5 and 10 Slime Lv.5, then return to Elara.");
    quests.back().addObjective  (3, std::make_shared<KillMonsterObjective>("Eye Lv.5", 10));
    quests.back().addObjective  (3, std::make_shared<KillMonsterObjective>("Slime Lv.5", 10));

    // Stage 4: Quay lại Elara báo cáo (Giai đoạn mới)
    quests.back().addNpcID      (4, 4); // NPC Elara
    quests.back().addDialogue   (4, "[1/4] You have returned. There is strength in you, I see that now.");
    quests.back().addDialogue   (4, "[2/4] Your blade is sharp enough for what is to come. I will trust you.");
    quests.back().addDialogue   (4, "[3/4] But Bren must be informed of our plan. Take this letter to him.");
    quests.back().addDialogue   (4, "[4/4] Go now. Do not let it fall into the wrong hands.");
    quests.back().addDescription(4, "You have proven your strength. Report back to Elara.");
    quests.back().addObjective  (4, std::make_shared<TalkObjective>(4));

    // Stage 5: Đem thư trả lại cho Bren
    quests.back().addNpcID      (5, 3); // NPC Bren
    quests.back().addDialogue   (5, "[1/4] Ah, you've returned with a message from Elara. Let me see...");
    quests.back().addDialogue   (5, "[2/4] (He reads the letter) Just as I feared... the Gazer Lord.");
    quests.back().addDialogue   (5, "[3/4] You have earned her trust. That is no small feat, hero.");
    quests.back().addDialogue   (5, "[4/4] Your path is clear now. Go back to Elara; she will guide you.");
    quests.back().addDescription(5, "Bring Elara's sealed letter back to Bren.");
    quests.back().addObjective  (5, std::make_shared<TalkObjective>(3));
    // --- [End] ---


    // --- [Begin] - [3] - The Secret of the Gloom-Swamp ---
    // Nhiệm vụ này vẫn giữ nguyên, bắt đầu sau khi Quest 2 hoàn thành.
    quests.push_back(Quest(3, "The Secret of the Gloom-Swamp", 1000));
    quests.back().addRequiredLevel(6);
    quests.back().addRequiredQuestID(2); // Yêu cầu hoàn thành quest "A Message for Elara"
    quests.back().addRequiredDescription("Speak to Elara in the Gloom-Swamp. Min Lv.6");

    // Stage 0: Gặp Elara để bắt đầu
    quests.back().addNpcID      (0, 4); // NPC Elara
    quests.back().addDialogue   (0, "[1/3] You have returned, and proven your might. I will trust you.");
    quests.back().addDialogue   (0, "[2/3] A great evil, the Gazer Lord, is the source of this poison.");
    quests.back().addDialogue   (0, "[3/3] To summon it, you must gather an 'Eye Orb' from the creatures here.");
    quests.back().addDescription(0, "Speak to Elara again to learn of the swamp's true secret.");
    quests.back().addObjective  (0, std::make_shared<TalkObjective>(4));

    // ... (Các giai đoạn còn lại của Quest 3 giữ nguyên như cũ)
    // Stage 1: Thu thập Eye Orb
    quests.back().addNpcID      (1, -1);
    quests.back().addDialogue   (1, std::string());
    quests.back().addDescription(1, "Collect an 'Eye Orb' from the swamp's creatures.");
    quests.back().addObjective  (1, std::make_shared<CollectItemObjective>("Eye Orb", 1));

    // Stage 2: Quay lại Elara
    quests.back().addNpcID      (2, 4);
    quests.back().addDialogue   (2, "[1/3] You have the orb... Its sorrow is potent.");
    quests.back().addDialogue   (2, "[2/3] Take this to the stone altar nearby and place it on the pedestal.");
    quests.back().addDialogue   (2, "[3/3] The Gazer Lord will be drawn to it. End its suffering, hero.");
    quests.back().addDescription(2, "Return to Elara with the Eye Orb.");
    quests.back().addObjective  (2, std::make_shared<TalkObjective>(4));

    // Stage 3: Triệu hồi và tiêu diệt Gazer Lord
    quests.back().addNpcID      (3, -1);
    quests.back().addDialogue   (3, std::string());
    quests.back().addDescription(3, "Use the swamp altar to summon and defeat the Gazer Lord.");
    quests.back().addObjective  (3, std::make_shared<KillMonsterObjective>("Gazer Lord Lv.10", 1)); // Boss

    // Stage 4: Báo cáo thành công với Elara
    quests.back().addNpcID      (4, 4); // NPC Elara
    quests.back().addDialogue   (4, "[1/3] The fog... it is lifting. The water seems clearer. You did it!");
    quests.back().addDialogue   (4, "[2/3] The source of all corruption lies beyond, guarded by The Gatekeeper.");
    quests.back().addDialogue   (4, "[3/3] Go back to Bren. Tell him. The final battle is drawing near.");
    quests.back().addDescription(4, "Return to Elara after defeating the Gazer Lord.");
    quests.back().addObjective  (4, std::make_shared<TalkObjective>(4));
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