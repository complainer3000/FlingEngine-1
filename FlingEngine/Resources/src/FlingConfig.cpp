#include "pch.h"
#include "FlingConfig.h"

namespace Fling
{

    INIReader FlingConfig::m_IniReader;

    void FlingConfig::Init()
    {
    }

    void FlingConfig::Shutdown()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    // Config file settings

    bool FlingConfig::LoadConfigFile(const std::string& t_File)
    {
        m_IniReader = INIReader(t_File);

        if (m_IniReader.ParseError() != 0)
        {
            F_LOG_ERROR("Cannot load config file: {} " , t_File);
            return false;
        }
        else
        {
            F_LOG_TRACE("Loaded config file {}", t_File);
        }
        return true;
    }

    std::string FlingConfig::GetString(const std::string& t_Section, const std::string& t_Key)
    {
        return m_IniReader.Get(t_Section, t_Key, "UNKNOWN");
    }

    int FlingConfig::GetInt(const std::string& t_Section, const std::string& t_Key)
    {
        return m_IniReader.GetInteger(t_Section, t_Key, -1);
    }

    bool FlingConfig::GetBool(const std::string& t_Section, const std::string& t_Key)
    {
        return m_IniReader.GetBoolean(t_Section, t_Key, false);
    }

    float FlingConfig::GetFloat(const std::string& t_Section, const std::string& t_Key)
    {
        return static_cast<float>(m_IniReader.GetReal(t_Section, t_Key, -1));
    }

    double FlingConfig::GetDouble(const std::string& t_Section, const std::string& t_Key)
    {
        return m_IniReader.GetReal(t_Section, t_Key, -1);
    }

    //////////////////////////////////////////////////////////////////////////
    // Command line parsing

    UINT32 FlingConfig::LoadCommandLineOpts(int argc, char* argv[])
    {
        UINT32 ArgsLoaded = 0;
        
        // Use regex to try and parse out if things are a key/val etc

        for (int i = 0; i < argc; ++i)
        {
            // Parse out if this is a key or not
            std::string value = argv[i];
            //F_LOG_TRACE("Command line options: {}", value);
        }

        return ArgsLoaded;
    }

}   // namespace Fling