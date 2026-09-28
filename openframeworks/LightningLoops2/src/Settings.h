#pragma once
#include "ofMain.h"

// settings.txt alternates label lines and value lines. Getters return the given
// default when a label is missing; setters only overwrite labels already in the file.
class Settings {

    public:
        bool load(const string& _name);
        bool save();

        int getInt(const string& key, int defaultValue) const;
        float getFloat(const string& key, float defaultValue) const;
        bool getBoolean(const string& key, bool defaultValue) const;
        string getString(const string& key, const string& defaultValue) const;
        glm::vec3 getVector(const string& key, const glm::vec3& defaultValue) const;
        glm::quat getQuaternion(const string& key, const glm::quat& defaultValue) const;

        void setFloat(const string& key, float f);
        void setString(const string& key, const string& s);
        void setVector(const string& key, const glm::vec3& p);
        void setQuaternion(const string& key, const glm::quat& q);

    private:
        int findValue(const string& key) const;
        vector<string> readList(const string& key) const;

        vector<string> data;
        string name = "settings.txt";

};
