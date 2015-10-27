#ifndef _LIGHTMANAGER_H
#define _LIGHTMANAGER_H

#include <stack>
using namespace std;

namespace graphics
{
    class LightIDManager
    {
    private:
            stack<unsigned int> lightids;
    public:
            LightIDManager(void)
            {
                    int i;

                    for (i=7;i>=0;i--)
                            lightids.push(i);
            }

            void freeLight(unsigned int id)
            {
                    lightids.push(id);
            }

            unsigned int getLightID()
            {
                    unsigned int next = lightids.top();
                    lightids.pop();
                    return next;
            }

            ~LightIDManager(void)
            {
            }
    };
}
#endif
