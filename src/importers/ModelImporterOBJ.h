#ifndef MODEL_IMPORTER_OBJ_H
#define MODEL_IMPORTER_OBJ_H

#include <map>
#include <math/vector.h>
#include <rendering/model.h>

namespace fps::importers {

   class ModelImporterOBJ
   {
      std::string m_strFile;

      public:
      ModelImporterOBJ(const std::string& strFile);

//      const auto& materials() const noexcept { return m_mMaterials; }
      bool import(std::map<std::string, rendering::model>&) noexcept;

      private:

      bool loadMTL(const std::string& strFile);
   };

}

#endif // MODEL_IMPORTER_OBJ_H
