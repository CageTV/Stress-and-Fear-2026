/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "Settings.h"

namespace Settings
{
	namespace
	{
		constexpr const char* kPlugin = "Stress and Fear.esp";
		constexpr unsigned    kTotalId = 0x000801;    // Stress_Total
		constexpr unsigned    kEnabledId = 0x0008A5;  // Stress_Enabled
		constexpr unsigned    kFearId = 0x0008A6;     // Stress_FearEnabled
		constexpr unsigned    kRateId = 0x0008A7;     // Stress_Rate
		constexpr unsigned    kBoxId = 0x0008A8;      // Stress_BoxMessages
		constexpr const char* kFile = "Data/SKSE/Plugins/StressAndFear/settings.ini";
		constexpr float       kWriteDelay = 0.6f;

		struct Config
		{
			float stress = 1.0f;
			float fear = 1.0f;
			float box = 1.0f;
			float rate = 1.0f;
			bool  applyOnLoad = true;
		};

		std::recursive_mutex mu;
		Config               cfg;
		bool                 fileExisted = false;
		bool                 dirty = false;
		float                timer = 0.0f;

		RE::TESGlobal* Find(unsigned a_id)
		{
			auto* dh = RE::TESDataHandler::GetSingleton();
			return dh ? dh->LookupForm<RE::TESGlobal>(a_id, kPlugin) : nullptr;
		}

		// The forms never go away once the data is loaded, so each one is looked up once.
#define GLOBAL_GETTER(name, id)                  \
	RE::TESGlobal* name()                        \
	{                                            \
		static RE::TESGlobal* g = nullptr;       \
		if (!g) {                                \
			g = Find(id);                        \
		}                                        \
		return g;                                \
	}
		GLOBAL_GETTER(TotalG, kTotalId)
		GLOBAL_GETTER(EnabledG, kEnabledId)
		GLOBAL_GETTER(FearG, kFearId)
		GLOBAL_GETTER(RateG, kRateId)
		GLOBAL_GETTER(BoxG, kBoxId)
#undef GLOBAL_GETTER

		float Clamp(float a_v, float a_lo, float a_hi) { return std::min(std::max(a_v, a_lo), a_hi); }

		std::string Trim(const std::string& a_s)
		{
			const auto b = a_s.find_first_not_of(" \t\r\n");
			const auto e = a_s.find_last_not_of(" \t\r\n");
			return b == std::string::npos ? std::string() : a_s.substr(b, e - b + 1);
		}

		bool ReadFile()
		{
			std::ifstream in(kFile);
			if (!in) {
				return false;
			}
			std::string line;
			while (std::getline(in, line)) {
				if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '[') {
					continue;
				}
				const auto eq = line.find('=');
				if (eq == std::string::npos) {
					continue;
				}
				const std::string key = Trim(line.substr(0, eq));
				float             f = 0.0f;
				try {
					f = std::stof(Trim(line.substr(eq + 1)));
				} catch (...) {
					continue;
				}
				if (key == "stress_enabled") {
					cfg.stress = f > 0.5f ? 1.0f : 0.0f;
				} else if (key == "fear_enabled") {
					cfg.fear = f > 0.5f ? 1.0f : 0.0f;
				} else if (key == "box_messages") {
					cfg.box = f > 0.5f ? 1.0f : 0.0f;
				} else if (key == "stress_rate") {
					cfg.rate = Clamp(f, 0.1f, 4.0f);
				} else if (key == "apply_on_load") {
					cfg.applyOnLoad = f > 0.5f;
				}
			}
			return true;
		}

		void WriteFile()
		{
			std::error_code ec;
			std::filesystem::create_directories(std::filesystem::path(kFile).parent_path(), ec);
			std::ofstream out(kFile, std::ios::trunc);
			if (!out) {
				SKSE::log::warn("Cannot write {}", kFile);
				return;
			}
			out << "; Stress and Fear 2026 settings. Written by the in-game pages; safe to edit by hand while the game is closed.\n"
				<< "stress_enabled=" << (cfg.stress > 0.5f ? 1 : 0) << "\n"
				<< "fear_enabled=" << (cfg.fear > 0.5f ? 1 : 0) << "\n"
				<< "box_messages=" << (cfg.box > 0.5f ? 1 : 0) << "\n"
				<< "stress_rate=" << cfg.rate << "\n"
				<< "apply_on_load=" << (cfg.applyOnLoad ? 1 : 0) << "\n";
			dirty = false;
			fileExisted = true;
		}

		void Touch()
		{
			dirty = true;
			timer = kWriteDelay;
		}

		void Put(RE::TESGlobal* a_g, float a_v)
		{
			if (a_g) {
				a_g->value = a_v;
			}
		}

		void PushGlobals()
		{
			Put(EnabledG(), cfg.stress);
			Put(FearG(), cfg.fear);
			Put(BoxG(), cfg.box);
			Put(RateG(), cfg.rate);
		}

		void PullGlobals()
		{
			if (auto* g = EnabledG()) {
				cfg.stress = g->value > 0.5f ? 1.0f : 0.0f;
			}
			if (auto* g = FearG()) {
				cfg.fear = g->value > 0.5f ? 1.0f : 0.0f;
			}
			if (auto* g = BoxG()) {
				cfg.box = g->value > 0.5f ? 1.0f : 0.0f;
			}
			if (auto* g = RateG()) {
				cfg.rate = g->value;
			}
		}
	}

	bool Loaded()
	{
		auto* dh = RE::TESDataHandler::GetSingleton();
		return dh && dh->LookupModByName(kPlugin) != nullptr;
	}

	bool Available() { return TotalG() && EnabledG() && FearG() && RateG() && BoxG(); }

	bool StressEnabled() { return EnabledG() && EnabledG()->value > 0.5f; }

	void SetStressEnabled(bool a_on)
	{
		std::scoped_lock lk(mu);
		cfg.stress = a_on ? 1.0f : 0.0f;
		Put(EnabledG(), cfg.stress);
		if (!a_on) {
			Put(TotalG(), 0.0f);  // the original menu clears the stress when the system is switched off
		}
		Touch();
	}

	bool FearEnabled() { return FearG() && FearG()->value > 0.5f; }

	void SetFearEnabled(bool a_on)
	{
		std::scoped_lock lk(mu);
		cfg.fear = a_on ? 1.0f : 0.0f;
		Put(FearG(), cfg.fear);
		Touch();
	}

	bool BoxMessages() { return BoxG() && BoxG()->value > 0.5f; }

	void SetBoxMessages(bool a_on)
	{
		std::scoped_lock lk(mu);
		cfg.box = a_on ? 1.0f : 0.0f;
		Put(BoxG(), cfg.box);
		Touch();
	}

	float Rate() { return RateG() ? RateG()->value : cfg.rate; }

	void SetRate(float a_rate)
	{
		std::scoped_lock lk(mu);
		cfg.rate = Clamp(a_rate, 0.1f, 4.0f);
		Put(RateG(), cfg.rate);
		Touch();
	}

	float Stress() { return TotalG() ? TotalG()->value : 0.0f; }

	void SetStress(float a_value) { Put(TotalG(), Clamp(a_value, 0.0f, 100.0f)); }

	bool ApplyOnLoad() { return cfg.applyOnLoad; }

	void SetApplyOnLoad(bool a_on)
	{
		std::scoped_lock lk(mu);
		cfg.applyOnLoad = a_on;
		Touch();
	}

	void OnDataLoaded()
	{
		std::scoped_lock lk(mu);
		fileExisted = ReadFile();
		SKSE::log::info("Settings: {}", fileExisted ? "settings.ini read" : "no settings.ini yet");
	}

	void OnGameLoaded()
	{
		std::scoped_lock lk(mu);
		if (!Available()) {
			SKSE::log::info("Game loaded: Stress and Fear.esp is not loaded, nothing to apply");
			return;
		}
		if (!fileExisted) {
			PullGlobals();  // first run: keep what the game has
			WriteFile();
			SKSE::log::info("Game loaded: settings.ini written from the game's values");
		} else if (cfg.applyOnLoad) {
			PushGlobals();
			SKSE::log::info("Game loaded: settings.ini applied to the globals");
		}
	}

	void SaveNow()
	{
		std::scoped_lock lk(mu);
		PullGlobals();
		WriteFile();
	}

	void LoadNow()
	{
		std::scoped_lock lk(mu);
		if (ReadFile()) {
			PushGlobals();
		}
	}

	void ResetDefaults()
	{
		std::scoped_lock lk(mu);
		const bool keep = cfg.applyOnLoad;
		cfg = Config{};
		cfg.applyOnLoad = keep;
		PushGlobals();
		Touch();
	}

	void Tick(float a_dt)
	{
		std::scoped_lock lk(mu);
		if (dirty) {
			timer -= a_dt;
			if (timer <= 0.0f) {
				WriteFile();
			}
		}
	}

	const char* FilePath() { return kFile; }
}
