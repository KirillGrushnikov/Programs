#include "LoadSettings.h"

Param& Settings::GetParam(const std::string& name)
{
    if (LoadParam)
    {
        // Только первый объект с заданным именем
        for (int i = 0; i < params.size(); i++)
            if (params[i].name == name)
                return params[i];
    }
}
std::vector<Param> Settings::GetParams(const std::string& name)
{
    if (LoadParam)
    {
        // Все объекты с заданным именем
        std::vector<Param> vec;
        for (int i = 0; i < params.size(); i++)
            if (params[i].name == name)
                vec.push_back(params[i]);

        return vec;
    }
}

std::vector<Param> Settings::GetAllParams()
{
    if (LoadParam)
        return params;
};


// загрузка настроек из файла
bool Settings::LoadFromFile(std::string filename)
{
    FileName = filename;
    TiXmlDocument InfoFile(FileName.c_str());

    if (!InfoFile.LoadFile())
    {
        std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
        return false;
    }

    TiXmlElement* Info;
    Info = InfoFile.FirstChildElement("Info");
    TiXmlElement* objectGroupElement;

    std::vector<Param> Objects;
    if (Info->FirstChildElement("objectgroup") != NULL)
    {
        objectGroupElement = Info->FirstChildElement("objectgroup");
        while (objectGroupElement)
        {
            // Контейнер <object>
            TiXmlElement* objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");

            while (objectElement)
            {
                // Получаем все данные
                std::string objectName;
                if (objectElement->Attribute("name") != NULL)
                {
                    objectName = objectElement->Attribute("name");
                }

                float objectParam = NULL;
                if (objectElement->Attribute("value") != NULL)
                {
                    objectParam = atof(objectElement->Attribute("value"));
                }

                Param par;
                par.name = objectName;
                par.param = objectParam;

                Objects.push_back(par);

                objectElement = objectElement->NextSiblingElement("object");
            }

            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
        params = Objects;
    }
    else
    {
        std::cout << "No object found..." << std::endl;
    }
    return true;
}

// сохранение настроек в файл
void Settings::Save()
{
    std::ofstream file(FileName);
    if (!file.is_open()) return;

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    file << "\n";
    file << "<Info>";
    file << "\n";
    file << "   <objectgroup id=\"1\" name=\"Settings\">";
    file << "\n";
    file << "       <object name=\"VolumeMusic\" value=\"" + std::to_string(GetParam("VolumeMusic").param) + "\" />";
    file << "\n";
    file << "       <object name=\"VolumeEffect\" value=\"" + std::to_string(GetParam("VolumeEffect").param) + "\" />";
    file << "\n";
    file << "       <object name=\"FullScreen\" value=\"" + std::to_string(GetParam("FullScreen").param) + "\" />";
    file << "\n";
    file << "   </objectgroup>";
    file << "\n\n";



    file << "   <objectgroup id=\"2\" name=\"Statistic\">";
    file << "\n";
    file << "       <object name=\"LastLevel\" value=\"" + std::to_string(GetParam("LastLevel").param) + "\" />";
    file << "\n";
    file << "       <object name=\"AllStrokes\" value=\"" + std::to_string(GetParam("AllStrokes").param) + "\" />";
    file << "\n";
    file << "       <object name=\"MeamCountStrokes\" value=\"" + std::to_string(GetParam("MeamCountStrokes").param) + "\" />";
    file << "\n";
    file << "   </objectgroup>";
    file << "\n";
    file << "</Info>";
    file.close();
}