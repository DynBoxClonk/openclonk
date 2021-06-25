#appendto Pipe

/* Ask the state */

public func IsDrainPipe(){ return pipe_state == PIPE_STATE_Drain; }

public func IsSourcePipe(){ return pipe_state == PIPE_STATE_Source; }
