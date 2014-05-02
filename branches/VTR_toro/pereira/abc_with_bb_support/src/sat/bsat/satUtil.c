/**CFile****************************************************************

  FileName    [satUtil.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [C-language MiniSat solver.]

  Synopsis    [Additional SAT solver procedures.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: satUtil.c,v 1.4 2005/09/16 22:55:03 casem Exp $]

***********************************************************************/

#include <stdio.h>
#include <assert.h>
#include "satSolver.h"
#include "extra.h"

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

struct clause_t
{
    int size_learnt;
    lit lits[0];
};

static inline int  clause_size( clause* c )  { return c->size_learnt >> 1; }
static inline lit* clause_begin( clause* c ) { return c->lits;             }

static void Sat_SolverClauseWriteDimacs( FILE * pFile, clause * pC, bool fIncrement );

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Write the clauses in the solver into a file in DIMACS format.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Sat_SolverWriteDimacs( sat_solver * p, char * pFileName, lit* assumptionsBegin, lit* assumptionsEnd, int incrementVars )
{
    FILE * pFile;
    void ** pClauses;
    int nClauses, i;

    // count the number of clauses
    nClauses = p->clauses.size + p->learnts.size;
    for ( i = 0; i < p->size; i++ )
        if ( p->levels[i] == 0 && p->assigns[i] != l_Undef )
            nClauses++;

    // start the file
    pFile = fopen( pFileName, "wb" );
    if ( pFile == NULL )
    {
        printf( "Sat_SolverWriteDimacs(): Cannot open the ouput file.\n" );
        return;
    }
    fprintf( pFile, "c CNF generated by ABC on %s\n", Extra_TimeStamp() );
    fprintf( pFile, "p cnf %d %d\n", p->size, nClauses );

    // write the original clauses
    nClauses = p->clauses.size;
    pClauses = p->clauses.ptr;
    for ( i = 0; i < nClauses; i++ )
        Sat_SolverClauseWriteDimacs( pFile, pClauses[i], incrementVars );

    // write the learned clauses
    nClauses = p->learnts.size;
    pClauses = p->learnts.ptr;
    for ( i = 0; i < nClauses; i++ )
        Sat_SolverClauseWriteDimacs( pFile, pClauses[i], incrementVars );

    // write zero-level assertions
    for ( i = 0; i < p->size; i++ )
        if ( p->levels[i] == 0 && p->assigns[i] != l_Undef )
            fprintf( pFile, "%s%d%s\n",
                     (p->assigns[i] == l_False)? "-": "",
                     i + (int)(incrementVars>0),
                     (incrementVars) ? " 0" : "");

    // write the assumptions
    if (assumptionsBegin) {
        for (; assumptionsBegin != assumptionsEnd; assumptionsBegin++) {
            fprintf( pFile, "%s%d%s\n",
                     lit_sign(*assumptionsBegin)? "-": "",
                     lit_var(*assumptionsBegin) + (int)(incrementVars>0),
                     (incrementVars) ? " 0" : "");
        }
    }

    fprintf( pFile, "\n" );
    fclose( pFile );
}   

/**Function*************************************************************

  Synopsis    [Writes the given clause in a file in DIMACS format.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Sat_SolverClauseWriteDimacs( FILE * pFile, clause * pC, bool fIncrement )
{
    lit * pLits = clause_begin(pC);
    int nLits = clause_size(pC);
    int i;

    for ( i = 0; i < nLits; i++ )
        fprintf( pFile, "%s%d ", (lit_sign(pLits[i])? "-": ""),  lit_var(pLits[i]) + (int)(fIncrement>0) );
    if ( fIncrement )
        fprintf( pFile, "0" );
    fprintf( pFile, "\n" );
}

/**Function*************************************************************

  Synopsis    [Writes the given clause in a file in DIMACS format.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Sat_SolverPrintStats( FILE * pFile, sat_solver * p )
{
    printf( "starts        : %d\n", (int)p->stats.starts );
    printf( "conflicts     : %d\n", (int)p->stats.conflicts );
    printf( "decisions     : %d\n", (int)p->stats.decisions );
    printf( "propagations  : %d\n", (int)p->stats.propagations );
    printf( "inspects      : %d\n", (int)p->stats.inspects );
}

/**Function*************************************************************

  Synopsis    [Returns a counter-example.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int * Sat_SolverGetModel( sat_solver * p, int * pVars, int nVars )
{
    int * pModel;
    int i;
    pModel = ALLOC( int, nVars );
    for ( i = 0; i < nVars; i++ )
    {
        assert( pVars[i] >= 0 && pVars[i] < p->size );
        pModel[i] = (int)(p->model.ptr[pVars[i]] == l_True);
    }
    return pModel;    
}

/**Function*************************************************************

  Synopsis    [Duplicates all clauses, complements unit clause of the given var.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Sat_SolverDoubleClauses( sat_solver * p, int iVar )
{
    clause * pClause;
    lit Lit, * pLits;
    int RetValue, nClauses, nVarsOld, nLitsOld, nLits, c, v;
    // get the number of variables
    nVarsOld = p->size;
    nLitsOld = 2 * p->size;
    // extend the solver to depend on two sets of variables
    sat_solver_setnvars( p, 2 * p->size );
    // duplicate implications
    for ( v = 0; v < nVarsOld; v++ )
        if ( p->assigns[v] != l_Undef )
        {
            Lit = nLitsOld + toLitCond( v, p->assigns[v]==l_False );
            if ( v == iVar )
                Lit = lit_neg(Lit);
            RetValue = sat_solver_addclause( p, &Lit, &Lit + 1 );
            assert( RetValue );
        }
    // duplicate clauses
    nClauses = vecp_size(&p->clauses);
    for ( c = 0; c < nClauses; c++ )
    {
        pClause = p->clauses.ptr[c];
        nLits = clause_size(pClause);
        pLits = clause_begin(pClause);
        for ( v = 0; v < nLits; v++ )
            pLits[v] += nLitsOld;
        RetValue = sat_solver_addclause( p, pLits, pLits + nLits );
        assert( RetValue );
        for ( v = 0; v < nLits; v++ )
            pLits[v] -= nLitsOld;
    }
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


