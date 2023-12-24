#pragma once

#include "Definitions.h"
#include "Includes.h"
#include "LCU.h"
#include "Utils.h"
#include "Misc.h"

class GameTab
{
private:
	static inline bool onOpen = true;

public:
	static void Render()
	{
		if (ImGui::BeginTabItem("Game"))
		{
			static std::string result;
			static std::string custom;

			static std::vector<std::pair<long, std::string>> gamemodes;

			if (onOpen)
			{
				if (gamemodes.empty())
				{
					std::string getQueues = LCU::Request("GET", "/lol-game-queues/v1/queues");
					Json::CharReaderBuilder builder;
					const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
					JSONCPP_STRING err;
					Json::Value root;
					if (reader->parse(getQueues.c_str(), getQueues.c_str() + static_cast<int>(getQueues.length()), &root, &err))
					{
						if (root.isArray())
						{
							for (Json::Value::ArrayIndex i = 0; i < root.size(); i++)
							{
								if (root[i]["queueAvailability"].asString() != "Available")
									continue;

								int64_t id = root[i]["id"].asInt64();
								std::string name = root[i]["name"].asString();
								name += " " + std::to_string(id);
								//std::cout << id << " " << name << std::endl;
								std::pair<long, std::string> temp = { id, name };
								gamemodes.emplace_back(temp);
							}

							std::ranges::sort(gamemodes, [](auto& left, auto& right) {
								return left.first < right.first;
								});
						}
					}
				}
			}

			static std::vector<std::string> firstPosition = { "UNSELECTED", "TOP", "JUNGLE", "MIDDLE", "BOTTOM", "UTILITY", "FILL" };
			static std::vector<std::string> secondPosition = { "UNSELECTED", "TOP", "JUNGLE", "MIDDLE", "BOTTOM", "UTILITY", "FILL" };

			static int gameID = 0;

			ImGui::Columns(4, nullptr, false);

			if (ImGui::Button("Quickplay"))
				gameID = Quickplay;

			if (ImGui::Button("Draft pick"))
				gameID = DraftPick;

			if (ImGui::Button("Solo/Duo"))
				gameID = SoloDuo;

			if (ImGui::Button("Flex"))
				gameID = Flex;

			ImGui::NextColumn();

			if (ImGui::Button("ARAM"))
				gameID = ARAM;

			if (ImGui::Button("Arena"))
				gameID = Arena;

			if (ImGui::Button("Clash"))
				gameID = Clash;

			if (ImGui::Button("None"))
				result = LCU::Request("DELETE", "https://127.0.0.1/lol-lobby/v2/lobby");

			ImGui::NextColumn();

			if (ImGui::Button("Practice Tool"))
			{
				custom =
					R"({"customGameLobby":{"configuration":{"gameMode":"PRACTICETOOL","gameMutator":"","gameServerRegion":"","mapId":11,"mutators":{"id":1},"spectatorPolicy":"AllAllowed","teamSize":1},"lobbyName":"KBot","lobbyPassword":null},"isCustom":true})";
			}

			if (ImGui::Button("Practice Tool 5v5"))
			{
				custom =
					R"({"customGameLobby":{"configuration":{"gameMode":"PRACTICETOOL","gameMutator":"","gameServerRegion":"","mapId":11,"mutators":{"id":1},"spectatorPolicy":"AllAllowed","teamSize":5},"lobbyName":"KBot","lobbyPassword":null},"isCustom":true})";
			}

			if (ImGui::Button("TFT Normal"))
				gameID = TFTNormal;

			if (ImGui::Button("TFT Ranked"))
				gameID = TFTRanked;

			ImGui::NextColumn();

			if (ImGui::Button("TFT Hyper Roll"))
				gameID = TFTHyperRoll;

			if (ImGui::Button("TFT Double Up"))
				gameID = TFTDoubleUp;

			if (ImGui::Button("TFT Tutorial"))
				gameID = TFTTutorial;

			static int indexGamemodes = -1;
			auto labelGamemodes = "All Gamemodes";
			if (indexGamemodes != -1)
				labelGamemodes = gamemodes[indexGamemodes].second.c_str();

			if (ImGui::BeginCombo("##combolGamemodes", labelGamemodes, 0))
			{
				for (size_t n = 0; n < gamemodes.size(); n++)
				{
					const bool is_selected = indexGamemodes == n;
					if (ImGui::Selectable(gamemodes[n].second.c_str(), is_selected))
						indexGamemodes = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();

			if (ImGui::Button("Create##gamemode"))
			{
				gameID = gamemodes[indexGamemodes].first;
			}

			ImGui::Columns(1);

			ImGui::Separator();

			ImGui::Columns(4, nullptr, false);

			if (ImGui::Button("Tutorial 1"))
				gameID = Tutorial1;

			if (ImGui::Button("Tutorial 2"))
				gameID = Tutorial2;

			if (ImGui::Button("Tutorial 3"))
				gameID = Tutorial3;

			ImGui::NextColumn();

			if (ImGui::Button("Intro Bots"))
				gameID = IntroBots;

			if (ImGui::Button("Beginner Bots"))
				gameID = BeginnerBots;

			if (ImGui::Button("Intermediate Bots"))
				gameID = IntermediateBots;

			ImGui::NextColumn();

			if (ImGui::Button("Custom Blind"))
				custom =
				R"({"customGameLobby":{"configuration":{"gameMode":"CLASSIC","gameMutator":"","gameServerRegion":"","mapId":11,"mutators":{"id":1},"spectatorPolicy":"AllAllowed","teamSize":5},"lobbyName":"KBot","lobbyPassword":null},"isCustom":true})";

			if (ImGui::Button("Custom ARAM"))
				custom =
				R"({"customGameLobby":{"configuration":{"gameMode":"ARAM","gameMutator":"","gameServerRegion":"","mapId":12,"mutators":{"id":1},"spectatorPolicy":"AllAllowed","teamSize":5},"lobbyName":"KBot","lobbyPassword":null},"isCustom":true})";

			//"id" 1- blind 2- draft -4 all random 6- tournament draft

			ImGui::NextColumn();

			static std::vector<std::pair<int, std::string>> botChamps;
			static size_t indexBots = 0; // Here we store our selection data as an index.
			auto labelBots = "Bot";
			if (!botChamps.empty())
				labelBots = botChamps[indexBots].second.c_str();
			if (ImGui::BeginCombo("##comboBots", labelBots, 0))
			{
				if (botChamps.empty())
				{
					std::string getBots = LCU::Request("GET", "https://127.0.0.1/lol-lobby/v2/lobby/custom/available-bots");
					Json::CharReaderBuilder builder;
					const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
					JSONCPP_STRING err;
					Json::Value root;
					if (reader->parse(getBots.c_str(), getBots.c_str() + static_cast<int>(getBots.length()), &root, &err))
					{
						if (root.isArray())
						{
							for (auto& i : root)
							{
								std::pair temp = { i["id"].asInt(), i["name"].asString() };
								botChamps.emplace_back(temp);
							}
							std::ranges::sort(botChamps, [](std::pair<int, std::string> a, std::pair<int, std::string> b) {
								return a.second < b.second;
								});
						}
					}
				}

				for (size_t n = 0; n < botChamps.size(); n++)
				{
					const bool is_selected = (indexBots == n);
					if (ImGui::Selectable(botChamps[n].second.c_str(), is_selected))
						indexBots = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			std::vector<std::string> difficulties = { "NONE", "EASY", "MEDIUM", "HARD", "UBER", "TUTORIAL", "INTRO" };
			static size_t indexDifficulty = 0; // Here we store our selection data as an index.
			const char* labelDifficulty = difficulties[indexDifficulty].c_str();

			if (ImGui::BeginCombo("##comboDifficulty", labelDifficulty, 0))
			{
				for (size_t n = 0; n < difficulties.size(); n++)
				{
					const bool is_selected = (indexDifficulty == n);
					if (ImGui::Selectable(difficulties[n].c_str(), is_selected))
						indexDifficulty = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			static int botTeam = 0;

			if (ImGui::Button("Add bot##addBot"))
			{
				if (botChamps.empty())
				{
					MessageBoxA(nullptr, "Pick the bots champion first", "Adding bots failed", MB_OK);
				}
				else
				{
					std::string team = botTeam ? R"(,"teamId":"200"})" : R"(,"teamId":"100"})";
					std::string body = R"({"botDifficulty":")" + difficulties[indexDifficulty] + R"(","championId":)" + std::to_string(
						botChamps[indexBots].first) + team;
					result = LCU::Request("POST", "https://127.0.0.1/lol-lobby/v1/lobby/custom/bots", body);
				}
			}
			ImGui::SameLine();
			ImGui::RadioButton("Blue", &botTeam, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Red", &botTeam, 1);

			ImGui::Columns(1);

			//ImGui::Separator();
			//static int inputGameID = 0;
			//ImGui::InputInt("##inputGameID:", &inputGameID, 1, 100);
			//ImGui::SameLine();
			//if (ImGui::Button("Submit##gameID"))
			//{
			//	gameID = inputGameID;
			//}

			// if pressed any button, gameID or custom changed
			if (gameID != 0 || !custom.empty())
			{
				std::string body;
				if (custom.empty())
				{
					body = R"({"queueId":)" + std::to_string(gameID) + "}";
				}
				else
				{
					body = custom;
					custom = "";
				}
				if (gameID == DraftPick || gameID == SoloDuo || gameID == Flex || gameID == Quickplay)
				{
					result = LCU::Request("POST", "https://127.0.0.1/lol-lobby/v2/lobby", body);

					LCU::Request("PUT", "/lol-lobby/v1/lobby/members/localMember/position-preferences",
						R"({"firstPreference":")" + firstPosition[S.gameTab.indexFirstRole]
						+ R"(","secondPreference":")" + secondPosition[S.gameTab.indexSecondRole] + "\"}");
				}
				else
				{
					result = LCU::Request("POST", "https://127.0.0.1/lol-lobby/v2/lobby", body);
				}

				gameID = 0;
			}

			ImGui::Separator();

			ImGui::Columns(2, nullptr, false);
			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 7));
			if (const char* labelFirstPosition = firstPosition[S.gameTab.indexFirstRole].c_str(); ImGui::BeginCombo(
				"##comboFirstPosition", labelFirstPosition, 0))
			{
				for (size_t n = 0; n < firstPosition.size(); n++)
				{
					const bool isSelected = (S.gameTab.indexFirstRole == n);
					if (ImGui::Selectable(firstPosition[n].c_str(), isSelected))
						S.gameTab.indexFirstRole = n;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			/*		ImGui::SameLine();
					ImGui::Text("Primary");*/

			ImGui::SameLine();
			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 7));
			const char* second_labelPosition = secondPosition[S.gameTab.indexSecondRole].c_str();
			if (ImGui::BeginCombo("##comboSecondPosition", second_labelPosition, 0))
			{
				for (size_t n = 0; n < secondPosition.size(); n++)
				{
					const bool isSelected = (S.gameTab.indexSecondRole == n);
					if (ImGui::Selectable(secondPosition[n].c_str(), isSelected))
						S.gameTab.indexSecondRole = n;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			//ImGui::SameLine();
			//ImGui::Text("Secondary");

			ImGui::SameLine();

			if (ImGui::Button("Pick roles"))
			{
				result = LCU::Request("PUT", "/lol-lobby/v1/lobby/members/localMember/position-preferences",
					R"({"firstPreference":")" + firstPosition[S.gameTab.indexFirstRole]
					+ R"(","secondPreference":")" + secondPosition[S.gameTab.indexSecondRole] + "\"}");
			}
			ImGui::SameLine();
			ImGui::HelpMarker("If you are already in a lobby you can use this button to pick the roles, or start a new lobby with the buttons above");

			ImGui::NextColumn();

			if (ImGui::Button("Change runes"))
			{
				result = ChangeRunesOpgg();
			}

			ImGui::SameLine();
			ImGui::Columns(2, nullptr, false);

			ImGui::Checkbox("Blue/Red Side notification", &S.gameTab.sideNotification);

			ImGui::Columns(1);

			ImGui::Separator();

			ImGui::Columns(3, nullptr, false);
			if (ImGui::Button("Start queue"))
			{
				result = LCU::Request("POST", "https://127.0.0.1/lol-lobby/v2/lobby/matchmaking/search");
			}
			ImGui::NextColumn();

			// if you press this during queue search you wont be able to start the queue again
			// unless you reenter the lobby :)
			if (ImGui::Button("Dodge"))
			{
				result = LCU::Request(
					"POST",
					R"(https://127.0.0.1/lol-login/v1/session/invoke?destination=lcdsServiceProxy&method=call&args=["","teambuilder-draft","quitV2",""])",
					"");
			}
			ImGui::SameLine();
			ImGui::HelpMarker("Dodges lobby instantly, you still lose LP, but you don't have to restart the client");
			ImGui::NextColumn();

			static std::vector<std::string> itemsMultiSearch = {
				"OP.GG", "U.GG", "PORO.GG", "Porofessor.gg"
			};
			const char* selectedMultiSearch = itemsMultiSearch[S.gameTab.indexMultiSearch].c_str();

			if (ImGui::Button("Multi-Search"))
			{
				result = MultiSearch(itemsMultiSearch[S.gameTab.indexMultiSearch]);
			}

			ImGui::SameLine();

			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 6));
			if (ImGui::BeginCombo("##comboMultiSearch", selectedMultiSearch, 0))
			{
				for (size_t n = 0; n < itemsMultiSearch.size(); n++)
				{
					const bool is_selected = (S.gameTab.indexMultiSearch == n);
					if (ImGui::Selectable(itemsMultiSearch[n].c_str(), is_selected))
						S.gameTab.indexMultiSearch = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Columns(1);

			ImGui::Separator();

			ImGui::Columns(3, nullptr, false);
			ImGui::Checkbox("Auto accept", &S.gameTab.autoAcceptEnabled);

			ImGui::NextColumn();

			static std::vector<std::pair<std::string, int>> itemsInvite = { {"**Default", 0} };
			static size_t itemIdxInvite = 0;
			auto labelInvite = itemsInvite[itemIdxInvite].first.c_str();

			if (ImGui::Button("Invite to lobby"))
			{
				std::string getFriends = LCU::Request("GET", "https://127.0.0.1/lol-chat/v1/friends");

				Json::CharReaderBuilder builder;
				const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
				JSONCPP_STRING err;
				Json::Value root;
				if (reader->parse(getFriends.c_str(), getFriends.c_str() + static_cast<int>(getFriends.length()), &root, &err))
				{
					if (root.isArray())
					{
						unsigned invitedCount = 0;
						for (auto& i : root)
						{
							if (i["groupId"].asInt() != itemsInvite[itemIdxInvite].second)
								continue;

							std::string friendSummId = i["summonerId"].asString();
							std::string inviteBody = "[{\"toSummonerId\":" + friendSummId + "}]";
							LCU::Request("POST", "https://127.0.0.1/lol-lobby/v2/lobby/invitations", inviteBody);
							invitedCount++;
						}
						result = "Invited " + std::to_string(invitedCount) + " friends to lobby";
					}
				}
			}

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::CalcTextSize(std::string(15, 'W').c_str(), nullptr, true).x);
			if (ImGui::BeginCombo("##comboInvite", labelInvite, 0))
			{
				std::string getGroups = LCU::Request("GET", "https://127.0.0.1/lol-chat/v1/friend-groups");
				Json::CharReaderBuilder builder;
				const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
				JSONCPP_STRING err;
				Json::Value root;
				if (reader->parse(getGroups.c_str(), getGroups.c_str() + static_cast<int>(getGroups.length()), &root, &err))
				{
					if (root.isArray())
					{
						itemsInvite.clear();
						for (auto& i : root)
						{
							std::pair temp = { i["name"].asString(), i["id"].asInt() };
							itemsInvite.emplace_back(temp);
						}
						std::ranges::sort(itemsInvite, [](std::pair<std::string, int> a, std::pair<std::string, int> b) { return a.second < b.second; });
					}
				}

				for (size_t n = 0; n < itemsInvite.size(); n++)
				{
					const bool is_selected = (itemIdxInvite == n);
					if (ImGui::Selectable(itemsInvite[n].first.c_str(), is_selected))
						itemIdxInvite = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::NextColumn();

			ImGui::Text("Nexus Blitz Force");

			ImGui::SameLine();

			if (ImGui::Button("Jungle"))
			{
				LCU::Request("PATCH", "/lol-champ-select/v1/session/my-selection", "{\"spell1Id\":4,\"spell2Id\":11}");
			}

			ImGui::SameLine();
			if (ImGui::Button("Lane"))
			{
				LCU::Request("PATCH", "/lol-champ-select/v1/session/my-selection", "{\"spell1Id\":4,\"spell2Id\":7}");
			}

			ImGui::Columns(1);

			ImGui::Separator();

			//ImGui::Columns(2, 0, false);

			ImGui::Text("Instant message:");
			ImGui::SameLine();
			static char bufInstantMessage[500];
			std::ranges::copy(S.gameTab.instantMessage, bufInstantMessage);
			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 6));
			ImGui::InputText("##inputInstantMessage", bufInstantMessage, IM_ARRAYSIZE(bufInstantMessage));
			S.gameTab.instantMessage = bufInstantMessage;

			ImGui::SameLine();
			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 7));
			ImGui::SliderInt("Delay##sliderInstantMessageDelay", &S.gameTab.instantMessageDelay, 0, 10000, "%d ms");
			ImGui::SameLine();

			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 10));
			ImGui::SliderInt("Time(s)##sliderInstantMessageTimes", &S.gameTab.instantMessageTimes, 1, 10, "%d");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 7));
			ImGui::SliderInt("Delay between##sliderInstantMessageDelayTimes", &S.gameTab.instantMessageDelayTimes, 0, 4000, "%d ms");

			ImGui::Separator();

			static bool isStillBeingFetched = true;
			if (!champSkins.empty())
				isStillBeingFetched = false;

			static ImGui::ComboAutoSelectData instalockComboData;
			static ImGui::ComboAutoSelectData backupPickComboData;

			if (onOpen)
			{
				std::vector<std::pair<int, std::string>> instalockChamps = GetInstalockChamps();

				if (!instalockChamps.empty())
				{
					std::vector<std::string> instalockChampsNames;
					std::vector<std::string> backupPickChampsNames;
					instalockChampsNames.reserve(instalockChamps.size() + 1);
					backupPickChampsNames.reserve(instalockChamps.size() + 1);
					instalockChampsNames.emplace_back("Random");
					backupPickChampsNames.emplace_back("None");

					if (S.gameTab.instalockId == -1)
					{
						std::ranges::copy("Random", instalockComboData.input);
					}
					else
					{
						std::string selectedChamp = ChampIdToName(S.gameTab.instalockId);
						std::ranges::copy(selectedChamp, instalockComboData.input);
					}

					if (S.gameTab.backupId == 0)
					{
						std::ranges::copy("None", backupPickComboData.input);
					}
					else
					{
						std::string selectedChamp = ChampIdToName(S.gameTab.backupId);
						std::copy(selectedChamp.begin(), selectedChamp.end(), backupPickComboData.input);
					}
					
					for (size_t i = 0; i < instalockChamps.size(); i++)
					{
						instalockChampsNames.emplace_back(instalockChamps[i].second);
						backupPickChampsNames.emplace_back(instalockChamps[i].second);
						if (instalockComboData.input == instalockChamps[i].second)
						{
							instalockComboData.index = i + 1;
						}
						if (backupPickComboData.input == instalockChamps[i].second)
						{
							backupPickComboData.index = i + 1;
						}
					}
					instalockComboData.items = instalockChampsNames;
					backupPickComboData.items = backupPickChampsNames;
				}
			}

			ImGui::RadioButton("Off", &S.gameTab.instalockEnabled, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Instalock", &S.gameTab.instalockEnabled, 1);
			ImGui::SameLine();
			ImGui::RadioButton("Instapick", &S.gameTab.instalockEnabled, 2);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 6));
			if (ImGui::ComboAutoSelect("##comboInstalock", instalockComboData))
			{
				if (instalockComboData.index != -1)
				{
					if (std::string(instalockComboData.input) == "Random")
					{
						S.gameTab.instalockId = -1;
					}
					else
					{
						for (const auto& [key, name, skins] : champSkins)
						{
							if (instalockComboData.input == name)
							{
								S.gameTab.instalockId = key;
							}
						}
					}
				}
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 6));

			ImGui::SliderInt("Delay##sliderInstalockDelay", &S.gameTab.instalockDelay, 0, 10000, "%d ms");

			ImGui::SameLine();

			ImGui::Checkbox("Dodge on champion ban", &S.gameTab.dodgeOnBan);

			ImGui::SameLine();
			ImGui::HelpMarker("Ignores backup pick");

			ImGui::Text("Backup pick:");
			ImGui::SameLine();

			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 6));
			if (ImGui::ComboAutoSelect("##comboBackupPick", backupPickComboData))
				if (backupPickComboData.index != -1 && backupPickComboData.index != 0) {
					for (const auto& champ : champSkins)
						if (backupPickComboData.input == champ.name)
							S.gameTab.backupId = champ.key;
				}
				else
					S.gameTab.backupId = 0;
			
			ImGui::SameLine();
			ImGui::HelpMarker("Select None or leave this blank to disable");

			ImGui::Columns(1);

			ImGui::Text("Autoban:");
			ImGui::SameLine();

			static ImGui::ComboAutoSelectData autobanComboData;
			if (onOpen)
			{
				std::vector<std::string> autobanChampsNames;
				if (!champSkins.empty())
				{
					autobanChampsNames.reserve(champSkins.size() + 1);
					autobanChampsNames.emplace_back("None");

					if (S.gameTab.autoBanId == 0)
					{
						std::ranges::copy("None", autobanComboData.input);
					}
					else
					{
						std::string selectedChamp = ChampIdToName(S.gameTab.autoBanId);
						std::ranges::copy(selectedChamp, autobanComboData.input);
					}

					for (size_t i = 0; i < champSkins.size(); i++)
					{
						autobanChampsNames.emplace_back(champSkins[i].name);

						if (autobanComboData.input == champSkins[i].name)
						{
							autobanComboData.index = i + 1;
						}
					}
					autobanComboData.items = autobanChampsNames;
				}
			}

			ImGui::SameLine();
			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 6));
			if (ImGui::ComboAutoSelect("##comboAutoban", autobanComboData))
			{
				if (autobanComboData.index != -1 && autobanComboData.index != 0)
				{
					for (const auto& [key, name, skins] : champSkins)
					{
						if (autobanComboData.input == name)
						{
							S.gameTab.autoBanId = key;
						}
					}
				}
				else
					S.gameTab.autoBanId = 0;
			}

			ImGui::SameLine();
			ImGui::HelpMarker("Select None or leave this blank to disable");

			ImGui::SameLine();
			ImGui::SetNextItemWidth(static_cast<float>(S.Window.width / 6));
			ImGui::SliderInt("Delay##sliderautoBanDelay", &S.gameTab.autoBanDelay, 0, 10000, "%d ms");

			ImGui::SameLine();

			ImGui::Checkbox("Instant Mute", &S.gameTab.instantMute);

			static Json::StreamWriterBuilder wBuilder;
			static std::string sResultJson;
			static char* cResultJson;

			if (!result.empty())
			{
				Json::CharReaderBuilder builder;
				const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
				JSONCPP_STRING err;
				Json::Value root;
				if (!reader->parse(result.c_str(), result.c_str() + static_cast<int>(result.length()), &root, &err))
					sResultJson = result;
				else
				{
					sResultJson = Json::writeString(wBuilder, root);
				}
				result = "";
			}

			if (!sResultJson.empty())
			{
				cResultJson = sResultJson.data();
				ImGui::InputTextMultiline("##gameResult", cResultJson, sResultJson.size() + 1, ImVec2(600, 300));
			}

			if (onOpen)
				onOpen = false;

			ImGui::EndTabItem();
		}
		else
		{
			onOpen = true;
		}
	}

	static std::vector<std::pair<int, std::string>> GetInstalockChamps()
	{
		std::vector<std::pair<int, std::string>> temp;

		std::string result = LCU::Request("GET", "https://127.0.0.1/lol-champions/v1/owned-champions-minimal");
		Json::CharReaderBuilder builder;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		JSONCPP_STRING err;
		Json::Value root;
		if (reader->parse(result.c_str(), result.c_str() + static_cast<int>(result.length()), &root, &err))
		{
			if (root.isArray())
			{
				for (auto& i : root)
				{
					if (i["freeToPlay"].asBool() == true || i["ownership"]["owned"].asBool() == true ||
						(i["ownership"].isMember("xboxGPReward") && i["ownership"]["xboxGPReward"].asBool() == true))
					{
						std::string loadScreenPath = i["baseLoadScreenPath"].asString();
						size_t nameStart = loadScreenPath.find("ASSETS/Characters/") + strlen("ASSETS/Characters/");
						std::string champName = loadScreenPath.substr(nameStart, loadScreenPath.find('/', nameStart) - nameStart);

						std::pair champ = { i["id"].asInt(), champName };
						temp.emplace_back(champ);
					}
				}
			}
		}
		std::ranges::sort(temp, [](std::pair<int, std::string> a, std::pair<int, std::string> b) { return a.second < b.second; });
		return temp;
	}

	static void InstantMessage(const bool instantMute = false, const bool sideNotification = false)
	{
		auto start = std::chrono::system_clock::now();
		while (true)
		{
			auto now = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = now - start;
			if (diff.count() > 10) // took 10 seconds and still didn't connect to chat
			{
				return;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			Json::Value root;
			Json::CharReaderBuilder builder;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			JSONCPP_STRING err;

			LCU::SetCurrentClientRiotInfo();
			std::string getChat = cpr::Get(cpr::Url{ std::format("https://127.0.0.1:{}/chat/v5/participants", LCU::riot.port) },
				cpr::Header{ Utils::StringToHeader(LCU::riot.header) }, cpr::VerifySsl{ false }).text;
			if (!reader->parse(getChat.c_str(), getChat.c_str() + static_cast<int>(getChat.length()), &root, &err))
			{
				continue;
			}

			const auto& participantsArr = root["participants"];
			if (!participantsArr.isArray())
			{
				continue;
			}
			if (participantsArr.size() <= 1)
			{
				continue;
			}

			std::string cid = "";
			for (auto& i : participantsArr)
			{
				if (i["cid"].asString().contains("champ-select"))
				{
					cid = i["cid"].asString();
					break;
				}
			}
			if (cid == "")
			{
				continue;
			}

			if (instantMute || sideNotification)
			{
				std::string champSelect = LCU::Request("GET", "/lol-champ-select/v1/session");
				Json::Value rootCSelect;
				if (!champSelect.empty() && champSelect.find("RPC_ERROR") == std::string::npos)
				{
					if (reader->parse(champSelect.c_str(), champSelect.c_str() + static_cast<int>(champSelect.length()), &rootCSelect, &err))
					{
						if (instantMute)
						{
							int localPlayerCellId = rootCSelect["localPlayerCellId"].asInt();
							for (Json::Value::ArrayIndex i = 0; i < rootCSelect["myTeam"].size(); i++)
							{
								Json::Value player = rootCSelect["myTeam"][i];
								if (player["cellId"].asInt() == localPlayerCellId)
									continue;

								LCU::Request("POST", "/lol-champ-select/v1/toggle-player-muted",
									std::format(R"({{"summonerId":{0},"puuid":"{1}","obfuscatedSummonerId":{2},"obfuscatedPuuid":"{3}"}})",
										player["summonerId"].asString(), player["puuid"].asString(),
										player["obfuscatedSummonerId"].asString(),
										player["obfuscatedPuuid"].asString()));

								/*	LCU::Request("POST", "/telemetry/v1/events/general_metrics_number",
										R"({"eventName":"champ_select_toggle_player_muted_clicked","value":"0","spec":"high","isLowSpecModeOn":"false"})");

									LCU::Request("POST", std::format("/lol-chat/v1/conversations/{}/messages", cid),
										std::format("{{\"body\":\"{} is muted.\",\"type\":\"celebration\"}}", "player"));
								*/
							}
						}

						if (sideNotification)
						{
							if (rootCSelect["myTeam"].isArray() && !rootCSelect["myTeam"].empty())
							{
								std::string notification = "You are on the ";
								if (rootCSelect["myTeam"][0]["team"].asInt() == 1)
									notification += "Blue Side";
								else
									notification += "Red Side";
								LCU::Request("POST", std::format("/lol-chat/v1/conversations/{}/messages", cid),
									R"({"body":")" + notification + R"(","type":"celebration"})");
							}
						}
					}
				}
			}

			if (S.gameTab.instantMessage.empty())
				return;

			const std::string request = "https://127.0.0.1/lol-chat/v1/conversations/" + cid + "/messages";
			const std::string body = R"({"type":"chat", "body":")" + std::string(S.gameTab.instantMessage) + R"("})";

			std::this_thread::sleep_for(std::chrono::milliseconds(S.gameTab.instantMessageDelay));

			now = std::chrono::system_clock::now();
			int numOfSent = 0;
			while (true)
			{
				for (; numOfSent < S.gameTab.instantMessageTimes; numOfSent++)
				{
					if (std::string error = LCU::Request("POST", request, body); error.find("errorCode") != std::string::npos)
					{
						break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(S.gameTab.instantMessageDelayTimes));
				}

				if (numOfSent >= S.gameTab.instantMessageTimes)
				{
					return;
				}

				diff = now - start;
				if (diff.count() > 10) // took 10 seconds and still not all messages sent
				{
					return;
				}
			}
		}
	}

	static void AutoAccept()
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (FindWindowA("RiotWindowClass", "League of Legends (TM) Client"))
			{
				// game is running, auto accept is not needed
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				continue;
			}

			if (!FindWindowA("RCLIENT", "League of Legends"))
			{
				continue;
			}

			if (S.gameTab.autoAcceptEnabled || S.gameTab.autoBanId != 0 ||
				(S.gameTab.dodgeOnBan && S.gameTab.instalockEnabled != 0) ||
				(S.gameTab.instalockEnabled != 0 && S.gameTab.instalockId) ||
				!S.gameTab.instantMessage.empty())
			{
				Json::Value rootSearch;
				Json::Value rootChampSelect;
				Json::Value rootSession;
				Json::CharReaderBuilder builder;
				const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
				JSONCPP_STRING err;

				cpr::Session session;
				session.SetVerifySsl(false);
				session.SetHeader(Utils::StringToHeader(LCU::league.header));
				session.SetUrl(std::format("https://127.0.0.1:{}/lol-lobby/v2/lobby/matchmaking/search-state", LCU::league.port));

				std::string getSearchState = session.Get().text;
				if (!reader->parse(getSearchState.c_str(), getSearchState.c_str() + static_cast<int>(getSearchState.length()), &rootSearch, &err))
				{
					continue;
				}

				static bool onChampSelect = true; //false when in champ select
				static int useBackupId = 0;
				static bool isPicked = false;

				if (rootSearch["searchState"].asString() != "Found") // not found, not in champ select
				{
					onChampSelect = true;
					useBackupId = 0;
					isPicked = false;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					continue;
				}

				session.SetUrl(std::format("https://127.0.0.1:{}/lol-champ-select/v1/session", LCU::league.port));
				std::string getChampSelect = session.Get().text;
				if (getChampSelect.find("RPC_ERROR") != std::string::npos) // game found but champ select error means queue pop
				{
					onChampSelect = true;
					useBackupId = 0;
					isPicked = false;
					if (S.gameTab.autoAcceptEnabled)
					{
						session.SetUrl(std::format("https://127.0.0.1:{}/lol-matchmaking/v1/ready-check/accept", LCU::league.port));
						session.SetBody("");
						session.Post();
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				else // in champ select
				{
					if (!reader->parse(getChampSelect.c_str(), getChampSelect.c_str() + static_cast<int>(getChampSelect.length()), &rootChampSelect,
						&err))
					{
						continue;
					}

					if (onChampSelect)
					{
						onChampSelect = false;

						if (!S.gameTab.instantMessage.empty() || S.gameTab.instantMute || S.gameTab.sideNotification)
						{
							std::thread instantMessageThread(&GameTab::InstantMessage, S.gameTab.instantMute, S.gameTab.sideNotification);
							instantMessageThread.detach();
						}
					}

					if ((S.gameTab.instalockEnabled != 0 || S.gameTab.autoBanId) && !isPicked)
					{
						// get own summid
						session.SetUrl(std::format("https://127.0.0.1:{}/lol-login/v1/session", LCU::league.port));
						std::string getSession = session.Get().text;
						if (!reader->parse(getSession.c_str(), getSession.c_str() + static_cast<int>(getSession.length()), &rootSession, &err))
						{
							continue;
						}

						const int cellId = rootChampSelect["localPlayerCellId"].asInt();
						for (Json::Value::ArrayIndex j = 0; j < rootChampSelect["actions"].size(); j++)
						{
							auto actions = rootChampSelect["actions"][j];
							if (!actions.isArray())
							{
								continue;
							}
							for (auto& action : actions)
							{
								// search for own actions
								if (action["actorCellId"].asInt() == cellId)
								{
									if (std::string actionType = action["type"].asString(); actionType == "pick"
										&& S.gameTab.instalockId && S.gameTab.instalockEnabled != 0)
									{
										// if haven't picked yet
										if (action["completed"].asBool() == false)
										{
											if (!isPicked)
											{
												std::this_thread::sleep_for(std::chrono::milliseconds(S.gameTab.instalockDelay));

												int currentPick = S.gameTab.instalockId;
												if (useBackupId)
													currentPick = useBackupId;

												if (S.gameTab.instalockId == -1)
												{
													std::vector<std::pair<int, std::string>> instalockChamps = GetInstalockChamps();
													currentPick = instalockChamps[Utils::RandomInt(0, instalockChamps.size() - 1)].first;
												}

												session.SetUrl(std::format("https://127.0.0.1:{}/lol-champ-select/v1/session/actions/{}",
													LCU::league.port,
													action["id"].asString()));
												if (S.gameTab.instalockEnabled == 1)
													session.SetBody(R"({"completed":true,"championId":)" + std::to_string(currentPick) + "}");
												else {
													session.SetBody(R"({"championId":)" + std::to_string(currentPick) + "}");
													isPicked = true;
												}
												session.Patch();
											}
										}
										else
										{
											isPicked = true;
										}
									}
									else if (actionType == "ban" && S.gameTab.autoBanId != 0)
									{
										if (action["completed"].asBool() == false)
										{
											std::this_thread::sleep_for(std::chrono::milliseconds(S.gameTab.autoBanDelay));

											session.SetUrl(std::format("https://127.0.0.1:{}/lol-champ-select/v1/session/actions/{}",
												LCU::league.port,
												action["id"].asString()));
											session.SetBody(R"({"completed":true,"championId":)" + std::to_string(S.gameTab.autoBanId) + "}");
											session.Patch();
										}
									}
								}
								// action that isn't our player, if dodge on ban enabled or backup pick
								else if ((S.gameTab.dodgeOnBan || S.gameTab.backupId) && S.gameTab.instalockEnabled != 0
									&& S.gameTab.instalockId && (S.gameTab.instalockId != -1))
								{
									if (isPicked)
										break;

									if (action["actorCellId"].asInt() == cellId)
										continue;

									if (action["type"].asString() == "ban" && action["completed"].asBool() == true)
									{
										if (action["championId"].asInt() == S.gameTab.instalockId)
										{
											if (S.gameTab.dodgeOnBan)
											{
												session.SetUrl(
													std::format("https://127.0.0.1:{}", LCU::league.port) +
													R"(/lol-login/v1/session/invoke?destination=lcdsServiceProxy&method=call&args=["","teambuilder-draft","quitV2",""])");
												session.SetBody("");
												session.Post();
											}
											else if (S.gameTab.backupId)
											{
												useBackupId = S.gameTab.backupId;
											}
										}
									}
									else if (action["type"].asString() == "pick" && action["completed"].asBool() == true)
									{
										if (S.gameTab.backupId && (action["championId"].asInt() == S.gameTab.instalockId))
										{
											useBackupId = S.gameTab.backupId;
										}
									}
								}
							}
						}
					}
					else // instalock or autoban not enabled, we do nothing in champ select
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					}
				}
			}
		}
	}

	static std::string MultiSearch(const std::string& website)
	{
		std::string names;
		std::string champSelect = LCU::Request("GET", "https://127.0.0.1/lol-champ-select/v1/session");
		if (!champSelect.empty() && champSelect.find("RPC_ERROR") == std::string::npos)
		{
			Json::CharReaderBuilder builder;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			JSONCPP_STRING err;
			Json::Value rootRegion;
			Json::Value rootCSelect;
			Json::Value rootSummoner;
			Json::Value rootPartcipants;

			std::wstring summNames;
			bool isRanked = false;

			if (reader->parse(champSelect.c_str(), champSelect.c_str() + static_cast<int>(champSelect.length()), &rootCSelect, &err))
			{
				auto teamArr = rootCSelect["myTeam"];
				if (teamArr.isArray())
				{
					for (auto& i : teamArr)
					{
						if (i["nameVisibilityType"].asString() == "HIDDEN")
						{
							isRanked = true;
							break;
						}

						std::string summId = i["summonerId"].asString();
						if (summId != "0")
						{
							std::string summoner = LCU::Request("GET", "https://127.0.0.1/lol-summoner/v1/summoners/" + summId);
							if (reader->parse(summoner.c_str(), summoner.c_str() + static_cast<int>(summoner.length()), &rootSummoner, &err))
							{
								summNames += Utils::StringToWstring(rootSummoner["gameName"].asString()) + L"%23" + Utils::StringToWstring(rootSummoner["tagLine"].asString()) + L",";
							}
						}
					}

					//	Ranked Lobby Reveal
					if (isRanked)
					{
						summNames = L"";

						LCU::SetCurrentClientRiotInfo();
						std::string participants = cpr::Get(
							cpr::Url{ std::format("https://127.0.0.1:{}/chat/v5/participants", LCU::riot.port) },
							cpr::Header{ Utils::StringToHeader(LCU::riot.header) }, cpr::VerifySsl{ false }).text;
						if (reader->parse(participants.c_str(), participants.c_str() + static_cast<int>(participants.length()), &rootPartcipants,
							&err))
						{
							auto participantsArr = rootPartcipants["participants"];
							if (participantsArr.isArray())
							{
								for (auto& i : participantsArr)
								{
									if (!i["cid"].asString().contains("champ-select"))
										continue;
									summNames += Utils::StringToWstring(i["game_name"].asString()) + L"%23" + Utils::StringToWstring(i["game_tag"].asString()) + L",";
								}
							}
						}
					}

					std::wstring region;
					if (website == "U.GG") // platformId (euw1, eun1, na1)
					{
						std::string getAuthorization = LCU::Request("GET", "/lol-rso-auth/v1/authorization");
						if (reader->parse(getAuthorization.c_str(), getAuthorization.c_str() + static_cast<int>(getAuthorization.length()),
							&rootRegion, &err))
						{
							region = Utils::StringToWstring(rootRegion["currentPlatformId"].asString());
						}
					}
					else // region code (euw, eune na)
					{
						std::string getRegion = LCU::Request("GET", "/riotclient/region-locale");
						if (reader->parse(getRegion.c_str(), getRegion.c_str() + static_cast<int>(getRegion.length()), &rootRegion, &err))
						{
							region = Utils::StringToWstring(rootRegion["webRegion"].asString());
						}
					}

					if (!region.empty())
					{
						if (summNames.empty())
							return "Failed to get summoner names";

						if (summNames.at(summNames.size() - 1) == L',')
							summNames.pop_back();

						std::wstring url;
						if (website == "OP.GG")
						{
							url = L"https://" + region + L".op.gg/multi/query=" + summNames;
						}
						else if (website == "U.GG")
						{
							url = L"https://u.gg/multisearch?summoners=" + summNames + L"&region=" + Utils::ToLower(region);
						}
						else if (website == "PORO.GG")
						{
							url = L"https://poro.gg/multi?region=" + Utils::ToUpper(region) + L"&q=" + summNames;
						}
						else if (website == "Porofessor.gg")
						{
							url = L"https://porofessor.gg/pregame/" + region + L"/" + summNames;
						}
						Utils::OpenUrl(url.c_str(), nullptr, SW_SHOW);
						return Utils::WstringToString(url);
					}
					return "Failed to get region";
				}
			}
		}

		return "Champion select not found";
	}

	static std::string ChangeRunesOpgg()
	{
		//std::string champSelect = LCU::Request("GET", "/lol-champ-select/v1/session");
		//if (champSelect.empty() || champSelect.find("RPC_ERROR") != std::string::npos)
		//{
		//	return "Champion select not found";
		//}

		Json::CharReaderBuilder builder;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		JSONCPP_STRING err;
		Json::Value rootCurrentPage;
		//Json::Value rootCSelect;

		//if (!reader->parse(champSelect.c_str(), champSelect.c_str() + static_cast<int>(champSelect.length()), &rootCSelect, &err))
		//{
		//	return "Failed to get champion select";
		//}

		std::string currentChampion = LCU::Request("GET", "/lol-champ-select/v1/current-champion");
		if (currentChampion == "0" || currentChampion.empty() || currentChampion.find("RPC_ERROR") != std::string::npos)
		{
			return "Champion not picked";
		}

		std::string currentChampionName;
		for (const auto& [key, name, skins] : champSkins)
		{
			if (std::stoi(currentChampion) == key)
			{
				currentChampionName = name;
				break;
			}
		}

		std::string getCurrentPage = LCU::Request("GET", "/lol-perks/v1/currentpage");
		if (!reader->parse(getCurrentPage.c_str(), getCurrentPage.c_str() + static_cast<int>(getCurrentPage.length()), &rootCurrentPage, &err))
		{
			return "Failed to get current rune page";
		}
		std::string currentPageId = rootCurrentPage["id"].asString();

		std::stringstream ssOpgg(cpr::Get(cpr::Url{ "https://www.op.gg/champions/" + Utils::ToLower(currentChampionName) }).text);
		std::vector<std::string> runes;
		std::string primaryPerk, secondaryPerk;

		std::string buf;
		while (ssOpgg >> buf)
		{
			if (runes.size() == 9)
				break;

			if (buf.find("src=\"https://opgg-static.akamaized.net/images/lol/perk") != std::string::npos
				|| buf.find("src=\"https://opgg-static.akamaized.net/meta/images/lol/perk") != std::string::npos)
			{
				if (buf.find("grayscale") != std::string::npos)
					continue;

				if (buf.find("/perkStyle/") != std::string::npos)
				{
					buf = buf.substr(buf.find("/perkStyle/") + strlen("/perkStyle/"), 4);
					if (primaryPerk.empty())
						primaryPerk = buf;
					else if (secondaryPerk.empty())
						secondaryPerk = buf;
				}
				else if (buf.find("/perk/") != std::string::npos)
				{
					buf = buf.substr(buf.find("/perk/") + strlen("/perk/"), 4);
					runes.emplace_back(buf);
				}
				else if (buf.find("/perkShard/") != std::string::npos)
				{
					buf = buf.substr(buf.find("/perkShard/") + strlen("/perkShard/"), 4);
					runes.emplace_back(buf);
				}
			}
		}
		if (runes.size() != 9 || primaryPerk.empty() || secondaryPerk.empty())
		{
			return "Failed to fetch op.gg runes";
		}

		LCU::Request("DELETE", "/lol-perks/v1/pages/" + currentPageId);

		Json::Value rootPage;
		rootPage["name"] = currentChampionName + " OP.GG";
		rootPage["primaryStyleId"] = primaryPerk;
		rootPage["subStyleId"] = secondaryPerk;
		rootPage["selectedPerkIds"] = Json::Value(Json::arrayValue);
		for (const std::string& rune : runes)
			rootPage["selectedPerkIds"].append(rune);
		rootPage["current"] = true;

		return LCU::Request("POST", "lol-perks/v1/pages", rootPage.toStyledString());
	}

	static std::string ChampIdToName(const int& id)
	{
		for (const auto& [key, name, skins] : champSkins)
		{
			if (id == key)
			{
				return name;
			}
		}
		return "";
	}

	// TODO: rewrite and move these to config file
	static std::string GetOldJWT(std::string accId, int& oldtimestamp)
	{
		char* pRoaming;
		size_t roamingLen;
		[[maybe_unused]] errno_t err = _dupenv_s(&pRoaming, &roamingLen, "APPDATA");
		std::string roaming = pRoaming;
		std::string filePath = roaming + "\\tempar.json";

		std::fstream file(filePath, std::ios_base::in);
		if (file.good())
		{
			std::string config;
			std::string temp;
			while (std::getline(file, temp))
			{
				config += temp + "\n";
			}

			Json::Value root;
			Json::CharReaderBuilder builder;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			JSONCPP_STRING local_err;

			if (reader->parse(config.c_str(), config.c_str() + static_cast<int>(config.length()), &root, &local_err))
			{
				if (auto t = root[accId]; !t.empty())
				{
					std::string oldJWT;
					if (auto t2 = root[accId]["time"]; !t2.empty())
						oldtimestamp = t2.asUInt();
					if (auto t2 = root[accId]["jwt"]; !t2.empty())
						oldJWT = t2.asString();
					file.close();
					return oldJWT;
				}
			}
		}
		file.close();
		return {};
	}

	// true if need new jwt
	static bool CheckJWT(std::string accId)
	{
		char* pRoaming;
		size_t roamingLen;
		[[maybe_unused]] errno_t err = _dupenv_s(&pRoaming, &roamingLen, "APPDATA");
		std::string roaming = pRoaming;
		std::string filePath = roaming + "\\tempar.json";
		unsigned timestamp = 0;

		std::fstream file(filePath, std::ios_base::in);
		if (file.good())
		{
			std::string config;
			std::string temp;
			while (std::getline(file, temp))
			{
				config += temp + "\n";
			}

			Json::Value root;
			Json::CharReaderBuilder builder;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			JSONCPP_STRING local_err;

			if (reader->parse(config.c_str(), config.c_str() + static_cast<int>(config.length()), &root, &local_err))
			{
				if (auto t = root[accId]; !t.empty())
				{
					std::string oldJWT; // CppEntityAssignedButNoRead
					if (auto t2 = root[accId]["time"]; !t2.empty())
						timestamp = t2.asUInt();
					if (auto t2 = root[accId]["jwt"]; !t2.empty())
						oldJWT = t2.asString();
				}
				else
					return true;
			}
		}
		file.close();

		// if old timestamp is still valid
		if (timestamp + 60 * 60 * 24 > time(nullptr))
		{
			return false;
		}
		return true;
	}

#pragma warning ( push )
#pragma warning (disable : 4996)
	static void SaveJWT(std::string accId, std::string jwt, unsigned timestamp)
	{
		char* pRoaming;
		size_t roamingLen;
		[[maybe_unused]] errno_t err = _dupenv_s(&pRoaming, &roamingLen, "APPDATA");
		std::string roaming = pRoaming;
		std::string filePath = roaming + "\\tempar.json";
		// if file doesn't exist, create new one with {} so it can be parsed
		if (!std::filesystem::exists(filePath))
		{
			std::ofstream file(filePath);
			file << "{}";
			file.close();
		}

		/** \
		* \deprecated Use CharReader and CharReaderBuilder.
		*/
		Json::Reader reader;
		Json::Value root;

		std::ifstream iFile(filePath);
		if (iFile.good())
		{
			if (reader.parse(iFile, root, false))
			{
				root[accId]["jwt"] = jwt;
				root[accId]["time"] = timestamp;

				if (!root.toStyledString().empty())
				{
					std::ofstream oFile(filePath);
					oFile << root.toStyledString() << std::endl;
					oFile.close();
				}
			}
		}
		iFile.close();
	}
#pragma warning( pop )
};
