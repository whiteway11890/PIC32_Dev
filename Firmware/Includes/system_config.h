#ifdef __cplusplus
extern "C"
  {
#endif

    /*********************************************************************
  Function:
	unsigned int SYSTEMConfigPerformance(unsigned int sys_clock)

  Description:
	The function sets the PB divider, the Flash Wait states and the DRM wait states to the optimum value.
    It also enables the cacheability for the K0 segment.

  PreCondition:
	None

  Parameters:
	sys_clock - system clock in Hz

  Output:
	the PB clock frequency in Hz

  Side Effects:
	Sets the PB and Flash Wait states

  Remarks:
	The interrupts are disabled briefly, the DMA is suspended and the system is unlocked while performing the operation.
    Upon return the previous status of the interrupts and the DMA are restored. The system is re-locked.

  Example:
	<code>
	SYSTEMConfigPerformance(72000000);
	</code>
 ********************************************************************/
extern inline unsigned int __attribute__((always_inline)) SYSTEMConfigPerformance(unsigned int sys_clock)
{
    // set up the wait states
    unsigned int pb_clk;
#ifdef _PCACHE
    unsigned int cache_status;
#endif
    unsigned int int_status;

    pb_clk = SYSTEMConfigWaitStatesAndPB(sys_clock);

    int_status=INTDisableInterrupts();

    mBMXDisableDRMWaitState();

#ifdef _PCACHE
    cache_status = mCheGetCon();
    cache_status |= CHE_CONF_PF_ALL;
    mCheConfigure(cache_status);
    CheKseg0CacheOn();
#endif

    INTRestoreInterrupts(int_status);

    return pb_clk;

}
    
#ifdef __cplusplus
  }
#endif 