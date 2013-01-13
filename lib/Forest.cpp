#include "pddby/Forest.h"

#include "Oak/Oak14.h"

namespace PddBy
{

IOakPtr Forest::CreateOak(std::string const& rootPath)
{
    return IOakPtr(new Oak14(Path(rootPath)));
}

} // namespace PddBy
