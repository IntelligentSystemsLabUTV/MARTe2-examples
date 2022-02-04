# RT-App: Finite State Machine

- The **StateMachine** is a key component which is used in many MARTe applications to synchronise the application state against the external environment.

- The **StateMachine** can be in one and only one state at a given time.

- Each state contains one, or more, **StateMachineEvent** elements.

- The **StateMachine** allows to associate the sending of **Messages** to **StateMachineEvents**.

- Upon receiving of a **Message**, the **StateMachine** will verify if the **Message** function is equal to the name of any of the declared **StateMachineEvent** elements for the current StateMachine state.
If it is, the **StateMachine** will change to the declared state and trigger any messages associated to this transition.

- 

``` ruby
+StateMachine = {
   Class = StateMachine
   +STATE1 = {
      Class = ReferenceContainer
      +GOTOSTATE2 = {
         Class = StateMachineEvent
         NextState = "STATE2"
         NextStateError = "ERROR"
         Timeout = 0
         +DoSomething = {
             Class = Message
             Destination = Receiver1
             Mode = ExpectsReply
             Function = DoSomething
             +Parameters = {
                 Class = ConfigurationDatabase
                 param = ...
             }
         }
         +DoSomethingElse = {
             Class = Message
             Destination = Receiver1
             Mode = ExpectsReply
             Function = DoSomethingElse
         }
      }
      +ERROR = {
         Class = StateMachineEvent
         NextState = "STATE3"
         NextStateError = "ERROR"
         Timeout = 0
         +DoSomething = {
             Class = Message
             Destination = Receiver1
             Mode = ExpectsReply
             Function = Function1
             +Parameters = {
                 Class = ConfigurationDatabase
                 param1 = 4
                 param2 = 5.312
             }
         }
      }
      ...
```
