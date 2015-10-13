// -------------------------------------------------------------- -*- C++ -*-   
// File: gotic.cpp                                                                
// --------------------------------------------------------------------------   
// Probleme GOTIC                                                      
// ---------------------                                                      


#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

int main(int argc, char** argv)
{
    IloEnv env ;

    try 
    {
        IloModel model(env) ;
        IloCplex cplex(env) ;

        // Get data                                                             
        // Import data from file
        try
        {
           //IloInt i, j ;

           const char* filename ;

           if (argc > 1)
              filename = argv[1] ;
           else
              filename = "data/tsp.dat" ;

           ifstream f(filename, ios::in) ;

            if (!f)
           {
              cerr << "No such file: " << filename << endl ;
              throw(1) ;
           }

        // Create data matrix
        IloArray<IloNumArray> costmatrix (env) ;
        f >> costmatrix ;
        IloInt n = costmatrix.getSize() ;

        // Define variables and "fill" them so as not to get seg faults
        IloArray<IloIntVarArray> x (env, n) ;
        for (i=0 ; i<n ; i++)
        {
            x[i] = IloIntVarArray (env, n) ;
            for(IloInt j=0 ; j<n ; j++)
            {
                x[i][j] = IloIntVar (env) ;
            }
        }

        IloIntVarArray u (env, n) ;
        for(i=0 ; i<n ; i++)
        {
            u[i] = IloIntVar (env) ;
        }

        // Define constraints
        IloExpr constr_i (env) ;
        IloExpr constr_j (env) ;
        IloExpr constr_u (env) ;
        
        // Vertical constraint
        // Create a full vector with a hole to express the vertical constraint as a scalar product
        IloIntArray basevector (env, n) ;
        basevector[0] = 0 ;
        for (int i=1 ; i<n ; i++)
        {
            basevector[i] = 1 ;
        }
        constr_i = IloScalProd(basevector, x[0]) ;
        model.add( constr_i == 1) ;

        for (i=1 ; i < n ; i++)
        {
            basevector[i-1] = 1 ;
            basevector[i] = 0 ;
            constr_i = IloScalProd(basevector, x[i]) ;
            model.add(constr_i == 1) ;

            for (j=0 ; j < n ; j++)
            {
               if (i != j)
               {
                  constr_j += x[j][i] ;
                  constr_u = u[i] - u[j] + n*x[i][j] ;
                  model.add(constr_u == 1) ;
               }
            }
            model.add(constr_j == 1) ;
        }


        // Define objective                                                     
        IloExpr obj (env) ;
        for (i=1 ; i < n ; i++)
        {
            for (j=0 ; j < n ; j++)
            {
               if (i != j)
               {
                  obj += costmatrix[i][j] * x[i][j] ;
               }
            }
        }                                             
        model.add( IloMinimize(env, obj) ) ;                                   

        // Extract model                                                        
        cplex.extract(model);

        // Export model                                                         

        // Set parameters                                                     

        // Solve                                                                
        if ( !cplex.solve() ) 
        {
            env.error() << "Failed to optimize problem" << endl;
            cplex.out() << "Solution status " << cplex.getStatus() << endl ;
            cplex.out() << "Model" << cplex.getModel() << endl ;
            throw(-1);
        }

        cplex.out() << "Solution status " << cplex.getStatus() << endl;
        cplex.out() << "Objective value " << cplex.getObjValue() << endl;

        // Print solution details                                               

    }
    catch (IloException& e) 
    {
        cerr << "Concert exception caught: " << e << endl;
    }

  }
  catch (...)
  {
      cerr << "Unknown exception caught" << endl ;
  }
    
  // freeing memory                                                           
  env.end();
  return 0;
} // END main         