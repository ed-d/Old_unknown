#include "gotic.h"
#include <ilcplex/ilocplex.h>

        
int solve_frontal (Data & data, Solution & sol)
{
    IloEnv env;

    // Le Big M
    Int M = /*au moins*/ 1000 ;
    M += 1000 ;

    // Variables et remplissage des variables
    // Première et dernière tâche
    // et temps d'arrivées
    IloArray<IloIntVarArray> f (env, data.get_wmax()) ;
    IloArray<IloIntVarArray> l (env, data.get_wmax()) ;
    IloArray<IloIntVarArray> t (env, data.get_wmax()) ;

    for (int w=0 ; w < data.get_wmax() ; w++)
    {
    	f[w] = IloIntVarArray (env, data.get_jmax()) ;
    	l[w] = IloIntVarArray (env, data.get_jmax()) ;
    	t[w] = IloIntVarArray (env, data.get_jmax()) ;    	

    	for (int j = 0 ; j < data.get_jmax() ; j++)
    	{
    		f[w][j] = IloIntVar (env, 0, 1) ;
    		l[w][j] = IloIntVar (env, 0, 1) ;
    		t[w][j] = IloIntVar (env, 0, 1) ;
    	}
    }

    // Affectateurs triples
    IloArray<IloArray<IloIntVarArray>> n (env, data.get_wmax()) ;
	for (int w=0 ; w < data.get_wmax() ; w++)
    {
    	n[w] = IloArray<IloIntVarArray> (env, data.get_jmax()) ;

    	for (int j = 0 ; j < data.get_jmax() ; j++)
    	{
    		n[w][j] = IloIntVarArray (env, data.get_jmax()) ;

    		for (int k = 0 ; k < data.get_jmax())
    		{
    			n[w][j][k] = IloIntVar (env, 0, 1) ;
    		}
    	}
    }






#ifdef NVERBOSE
    env.setOut (env.getNullStream ());   // no output on screen
#endif        

    return 0 ;
}

int solve_lagrangian (Data & data, Solution & sol)
{
    IloEnv       env;
#ifdef NVERBOSE
    env.setOut (env.getNullStream ());   // no output on screen
#endif        

    return 0;
}
