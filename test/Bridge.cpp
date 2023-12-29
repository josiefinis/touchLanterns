class Bridge
{
    public:
        uint8_t getState( const Lantern& lantern )
        {
            return *( lantern.state );
        }
        const SensorInput& getInput( const Lantern& lantern )
        {
            return lantern.input;
        }
        uint8_t getParent( const LanternCollection& lanternCollection, const Lantern& lantern )
        {
            const Lantern* parent = lantern.parent;
            if ( parent == nullptr ) { return 0xFF; }
            const Lantern* collection = lanternCollection.collection;
            int step = &collection[1] - collection;
            return ( parent - collection ) / step;
        }
        uint8_t getDelay( const Lantern& lantern )
        {
            return lantern.delay;
        }
};
