#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>

const std::set<std::string> UNIVERSAL_TRASH = {
    "utm_source", "utm_medium", "utm_campaign", "utm_term", "utm_content",
    "gclid", "fbclid", "yclid", "msclkid", "mc_eid",
    "source_ve_path", "embeds_referring_euri", "embeds_referring_origin",
    "_openstat", "from", "callback"
};

std::string clean_any_url(const std::string& url) {
    size_t q_pos = url.find('?');
    if (q_pos == std::string::npos) return url;

    std::string base = url.substr(0, q_pos);
    std::string query = url.substr(q_pos + 1);
    
    size_t hash_pos = query.find('#');
    std::string anchor = "";
    if (hash_pos != std::string::npos) {
        anchor = query.substr(hash_pos);
        query = query.substr(0, hash_pos);
    }

    std::vector<std::string> clean_params;
    std::stringstream ss(query);
    std::string pair;

    while (std::getline(ss, pair, '&')) {
        if (pair.empty()) continue;
        
        size_t eq_pos = pair.find('=');
        std::string key = (eq_pos == std::string::npos) ? pair : pair.substr(0, eq_pos);
        
        std::string lower_key = key;
        std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(), ::tolower);

        if (UNIVERSAL_TRASH.find(lower_key) == UNIVERSAL_TRASH.end()) {
            clean_params.push_back(pair);
        }
    }

    std::string result = base;
    if (!clean_params.empty()) {
        result += "?";
        for (size_t i = 0; i < clean_params.size(); ++i) {
            result += clean_params[i] + (i < clean_params.size() - 1 ? "&" : "");
        }
    }
    return result + anchor;
}