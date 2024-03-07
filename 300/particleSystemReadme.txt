1) Emit new particles if any
	a) Dispatch Emitter
	b) Update Count --> Count += newly emitted particles
2) Dispatch Particles
3) Get Subdata from SSBO
	a) Remove expired particles
	b) Update the count
4) Subdata into SSBO
