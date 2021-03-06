// Caramel C++ Library - Statechart Facility - Implementation

#include "CaramelPch.h"

#include "Statechart/StateImpl.h"
#include "Statechart/StateMachineImpl.h"
#include "Statechart/Transition.h"
#include <Caramel/Error/CatchException.h>
#include <Caramel/Functional/ScopeExit.h>
#include <Caramel/Task/TaskPoller.h>
#include <Caramel/Thread/ThisThread.h>


namespace Caramel
{

namespace Statechart
{

//
// Contents
//
//   StateMachine
//   State
//

///////////////////////////////////////////////////////////////////////////////
//
// State Machine
//

StateMachine::StateMachine( const std::string& name )
    : m_impl( new StateMachineImpl( name ))
{
}


StateMachine::~StateMachine()
{
}


//
// Operations
//

State StateMachine::AddState( Int stateId )
{
    StatePtr newState = std::make_shared< StateImpl >( stateId, m_impl->m_name );
    if ( ! m_impl->m_states.Insert( stateId, newState ))
    {
        CARAMEL_THROW( "State duplicate, machine: %s, stateId: %d", m_impl->m_name, stateId );
    }
    return State( newState );
}


void StateMachine::Initiate( Int stateId )
{
    StatePtr initialState;
    if ( ! m_impl->m_states.Find( stateId, initialState ))
    {
        CARAMEL_THROW( "State not found, machine: %s, stateId: %d", m_impl->m_name, stateId );
    }

    Task task(
        Sprintf( "Machine[%s].ProcessInitiate[%d]", m_impl->m_name, stateId ),
        [=] { m_impl->ProcessInitiate( initialState ); }
    );

    m_impl->m_taskExecutor->Submit( task );
}


void StateMachine::PostEvent( Int eventId )
{
    Task task(
        Sprintf( "Machine[%s].ProcessEvent[%d]", m_impl->m_name, eventId ),
        [=] { m_impl->ProcessEvent( eventId ); }
    );

    m_impl->m_taskExecutor->Submit( task );
}


void StateMachine::Process( const Ticks& sliceTicks )
{
    CARAMEL_ASSERT( m_impl->m_builtinTaskPoller );

    m_impl->m_builtinTaskPoller->PollFor( sliceTicks );
}


//
// Properties
//

Int StateMachine::GetCurrentStateId() const
{
    return m_impl->m_currentState->GetId();
}


//
// Implementation
//

StateMachineImpl::StateMachineImpl( const std::string& name )
    : m_name( name )
    , m_taskExecutor( nullptr )
    , m_builtinTaskPoller( new TaskPoller )
    , m_transitNumber( 0 )
{
    m_taskExecutor = m_builtinTaskPoller.get();
}


void StateMachineImpl::ProcessInitiate( StatePtr initialState )
{
    auto ulock = UniqueLock( m_mutex );

    m_actionThreadId = ThisThread::GetId();
    auto guard = ScopeExit( [=] { m_actionThreadId = ThreadId(); } );

    m_currentState = initialState;

    this->EnterState();
}


void StateMachineImpl::ProcessEvent( Int eventId )
{
    auto ulock = UniqueLock( m_mutex );

    m_actionThreadId = ThisThread::GetId();
    auto guard = ScopeExit( [=] { m_actionThreadId = ThreadId(); } );

    TransitionPtr transition;
    if ( m_currentState->m_transitions.Find( eventId, transition ))
    {
        StatePtr targetState;
        CARAMEL_VERIFY( m_states.Find( transition->targetStateId, targetState ));

        this->DoTransit( targetState );
        return;
    }

    // Otherwise, discard this event
    CARAMEL_TRACE_DEBUG( "%s discards event %d", m_currentState->GetName(), eventId );
}


void StateMachineImpl::DoTransit( StatePtr targetState )
{
    // REMARKS: At this point, the m_mutex should have been locked.

    this->ExitState();

    // TODO: Add transition action here

    m_currentState = targetState;

    this->EnterState();
}


void StateMachineImpl::StartTimer( const Ticks& ticks )
{
    CARAMEL_NOT_IMPLEMENTED();
}


//
// State Actions
//

void StateMachineImpl::EnterState()
{
    ++ m_transitNumber;
    m_currentStartTime = TickClock::Now();

    if ( m_currentState->m_enterAction )
    {
        auto xc = CatchException( [=] { m_currentState->m_enterAction(); } );
        if ( xc )
        {
            CARAMEL_TRACE_WARN( "%s enter action throws", m_currentState->GetName() );
        }
    }

    if ( Ticks::Zero() < m_currentState->m_autoTimerDuration )
    {
        this->StartTimer( m_currentState->m_autoTimerDuration );
    }
}


void StateMachineImpl::ExitState()
{
    // TODO: Cancel the timer.

    if ( m_currentState->m_exitAction )
    {
        auto xc = CatchException( [=] { m_currentState->m_exitAction(); } );
        if ( xc )
        {
            CARAMEL_TRACE_WARN( "%s exit action throws", m_currentState->GetName() );
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// State
//

State::State( StatePtr impl )
    : m_impl( impl )
{
}


State& State::EnterAction( Action action )
{
    CARAMEL_ASSERT( ! m_impl->m_enterAction );

    m_impl->m_enterAction = action;
    return *this;
}


State& State::ExitAction( Action action )
{
    CARAMEL_ASSERT( ! m_impl->m_exitAction );

    m_impl->m_exitAction = action;
    return *this;
}


State& State::Transition( Int eventId, Int targetStateId )
{
    auto transition = std::make_shared< Statechart::Transition >( targetStateId );
    if ( ! m_impl->m_transitions.Insert( eventId, transition ))
    {
        CARAMEL_THROW(
            "%s transition duplicate, eventId: %d, targetStateId: %d",
            m_impl->GetName(), eventId, targetStateId
        );
    }

    return *this;
}


//
// Implementation
//

StateImpl::StateImpl( Int stateId, const std::string& machineName )
    : m_id( stateId )
    , m_name( Sprintf( "Machine[%s].State[%d]", machineName, stateId ))
{
}


///////////////////////////////////////////////////////////////////////////////

} // namespace Statechart

} // namespace Caramel
