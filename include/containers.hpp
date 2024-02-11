/// Containers and container_only related methods
/// @Tommy0x121 @attention @todo #2 please reorganize: all struct here and all
///

#pragma once

struct ETL_container
{
    /* data */
};



/// @brief this is proto class 4 later reorganization
class containers
{
private:
    ETL_container etl; /// @todo leave this alone 4 now just example
public:
    /// @brief just allocate mem constructor, cpp'll handle itself
    containers(/* args */);

    /// @brief use defalut destructor, we're sane
    virtual ~containers() = default;

    /// @brief it'll be collection of overloaded fcns?
    /// @param etl @typedef ETL_container
    void Set_ETL(ETL_container &etl);
};

containers::containers(/* args */)
{
}

containers::~containers()
{
}

void containers::Set_ETL(ETL_container &etl)
{

}