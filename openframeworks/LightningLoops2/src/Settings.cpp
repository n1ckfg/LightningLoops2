#include "Settings.h"

// Shortest decimal that reads back as the same float, so saving doesn't drift values.
static string formatFloat(float f) {
    char buf[32];
    for (int precision = 6; precision < 9; precision++) {
        snprintf(buf, sizeof(buf), "%.*g", precision, f);
        if (strtof(buf, nullptr) == f) return buf;
    }
    snprintf(buf, sizeof(buf), "%.9g", f);
    return buf;
}

bool Settings::load(const string& _name) {
    name = _name;
    data.clear();

    std::ifstream file(ofToDataPath(name));
    if (!file) {
        ofLogWarning("Settings") << "Couldn't load settings file. Using defaults.";
        return false;
    }

    string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        data.push_back(line);
    }
    return true;
}

bool Settings::save() {
    std::ofstream file(ofToDataPath(name));
    for (const string& line : data) {
        file << line << "\n";
    }
    if (!file) {
        ofLogError("Settings") << "Failed to write settings file.";
        return false;
    }
    ofLogNotice("Settings") << "Saved " << ofToDataPath(name);
    return true;
}

// ~ ~ ~ GET ~ ~ ~

// Index of the value line under a label, or -1. Labels are matched ignoring
// surrounding whitespace ("Activity Threshold " has a trailing space).
int Settings::findValue(const string& key) const {
    for (size_t i = 0; i + 1 < data.size(); i++) {
        if (ofTrim(data[i]) == key) return i + 1;
    }
    return -1;
}

// Comma-separated components, ignoring spaces and parentheses.
vector<string> Settings::readList(const string& key) const {
    int i = findValue(key);
    if (i < 0) return {};

    string cleaned;
    for (char c : data[i]) {
        if (c != ' ' && c != '(' && c != ')') cleaned += c;
    }

    vector<string> parts = ofSplitString(cleaned, ",");
    while (!parts.empty() && parts.back().empty()) parts.pop_back();
    return parts;
}

int Settings::getInt(const string& key, int defaultValue) const {
    int i = findValue(key);
    return i < 0 ? defaultValue : ofToInt(data[i]);
}

float Settings::getFloat(const string& key, float defaultValue) const {
    int i = findValue(key);
    return i < 0 ? defaultValue : ofToFloat(data[i]);
}

bool Settings::getBoolean(const string& key, bool defaultValue) const {
    int i = findValue(key);
    return i < 0 ? defaultValue : ofToBool(ofTrim(data[i]));
}

string Settings::getString(const string& key, const string& defaultValue) const {
    int i = findValue(key);
    return i < 0 ? defaultValue : data[i];
}

glm::vec3 Settings::getVector(const string& key, const glm::vec3& defaultValue) const {
    if (findValue(key) < 0) return defaultValue;
    vector<string> parts = readList(key);
    glm::vec3 p(0);
    for (size_t i = 0; i < parts.size() && i < 3; i++) {
        p[i] = ofToFloat(parts[i]);
    }
    return p;
}

// Stored as w, x, y, z. Anything other than four components reads as no rotation.
glm::quat Settings::getQuaternion(const string& key, const glm::quat& defaultValue) const {
    if (findValue(key) < 0) return defaultValue;
    vector<string> parts = readList(key);
    if (parts.size() < 4) return glm::quat(1, 0, 0, 0);
    return glm::quat(ofToFloat(parts[0]), ofToFloat(parts[1]), ofToFloat(parts[2]), ofToFloat(parts[3]));
}

// ~ ~ ~ SET ~ ~ ~

void Settings::setFloat(const string& key, float f) {
    int i = findValue(key);
    if (i >= 0) data[i] = formatFloat(f);
}

void Settings::setString(const string& key, const string& value) {
    int i = findValue(key);
    if (i >= 0) data[i] = value;
}

void Settings::setVector(const string& key, const glm::vec3& p) {
    int i = findValue(key);
    if (i >= 0) data[i] = formatFloat(p.x) + ", " + formatFloat(p.y) + ", " + formatFloat(p.z);
}

void Settings::setQuaternion(const string& key, const glm::quat& q) {
    int i = findValue(key);
    if (i >= 0) data[i] = formatFloat(q.w) + ", " + formatFloat(q.x) + ", " + formatFloat(q.y) + ", " + formatFloat(q.z);
}
