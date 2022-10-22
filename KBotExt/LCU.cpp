#include <json/json.h>
#include <thread>

#include "LCU.h"
#include "HTTP.h"

std::string LCU::Request(const std::string& method, const std::string& endpoint, const std::string& body)
{
	std::string sURL = endpoint;
	if (sURL.find("https://127.0.0.1") == std::string::npos)
	{
		if (sURL.find("https://") == std::string::npos && sURL.find("http://") == std::string::npos)
		{
			while (sURL[0] == ' ')
				sURL.erase(sURL.begin());
			if (sURL[0] != '/')
				sURL.insert(0, "/");
			sURL.insert(0, "https://127.0.0.1");
		}
	}
	return HTTP::Request(method, sURL, body, league.header, "", "", league.port);
}

bool LCU::SetRiotClientInfo(const ClientInfo& info)
{
	riot = info;

	if (riot.port == 0 || riot.token == "")
		return false;

	riot.header = Auth::MakeRiotHeader(info);

	return true;
}

bool LCU::SetRiotClientInfo()
{
	return SetRiotClientInfo(Auth::GetClientInfo(Auth::GetProcessId(L"RiotClientUx.exe")));
}

bool LCU::SetLeagueClientInfo(const ClientInfo& info)
{
	league = info;

	if (league.port == 0 || league.token == "")
		return false;

	league.header = Auth::MakeLeagueHeader(info);

	return true;
}

bool LCU::SetLeagueClientInfo()
{
	if (!LCU::IsProcessGood())
		return false;
	return SetLeagueClientInfo(Auth::GetClientInfo(LCU::leagueProcesses[LCU::indexLeagueProcesses].first));
}

void LCU::GetLeagueProcesses()
{
	std::vector<DWORD>allProcessIds = Auth::GetAllProcessIds(L"LeagueClientUx.exe");
	// remove unexisting clients
	for (size_t i = 0; i < LCU::leagueProcesses.size(); i++)
	{
		bool exists = false;
		for (const DWORD& proc : allProcessIds)
		{
			if (proc == LCU::leagueProcesses[i].first)
			{
				exists = true;
				break;
			}
		}
		if (!exists)
		{
			LCU::leagueProcesses.erase(LCU::leagueProcesses.begin() + i);
			if (i == LCU::indexLeagueProcesses)
				LCU::SetLeagueClientInfo();
		}
	}

	for (const DWORD& proc : allProcessIds)
	{
		int foundIndex = -1;
		for (size_t i = 0; i < LCU::leagueProcesses.size(); i++)
		{
			if (LCU::leagueProcesses[i].first == proc)
			{
				foundIndex = static_cast<int>(i);
				break;
			}
		}

		if (foundIndex != -1 && !LCU::leagueProcesses[foundIndex].second.empty())
			continue;

		ClientInfo currInfo = Auth::GetClientInfo(proc);
		currInfo.header = Auth::MakeLeagueHeader(currInfo);

		size_t currentIndex = foundIndex;
		if (foundIndex == -1)
		{
			currentIndex = LCU::leagueProcesses.size();
			std::pair<DWORD, std::string>temp = { proc, "" };
			LCU::leagueProcesses.emplace_back(temp);
		}

		std::thread t([currentIndex, currInfo]()
			{
				short sessionFailCount = 0;
				while (true)
				{
					std::string procSession = HTTP::Request("GET", "https://127.0.0.1/lol-login/v1/session", "", currInfo.header, "", "", currInfo.port);

					// probably legacy client
					if (procSession.find("errorCode") != std::string::npos)
					{
						sessionFailCount++;
						if (sessionFailCount > 5)
						{
							LCU::leagueProcesses[currentIndex].second = "!FAILED!";
							break;
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(300));
						continue;
					}

					Json::CharReaderBuilder builder;
					const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
					JSONCPP_STRING err;
					Json::Value root;

					if (reader->parse(procSession.c_str(), procSession.c_str() + static_cast<int>(procSession.length()), &root, &err))
					{
						std::string currSummId = root["summonerId"].asString();
						// player has summId when client is loaded
						if (!currSummId.empty())
						{
							std::string currSummoner = HTTP::Request("GET", "https://127.0.0.1/lol-summoner/v1/summoners/" + currSummId,
								"", currInfo.header, "", "", currInfo.port);
							if (reader->parse(currSummoner.c_str(), currSummoner.c_str() + static_cast<int>(currSummoner.length()), &root, &err))
							{
								LCU::leagueProcesses[currentIndex].second = std::string(root["displayName"].asString());
								break;
							}
						}
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(300));
				}
			});
		t.detach();
	}
}

bool LCU::IsProcessGood()
{
	return !LCU::leagueProcesses.empty() && LCU::indexLeagueProcesses < LCU::leagueProcesses.size();
}