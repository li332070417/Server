class CNoncopyable
{
public:
	CNoncopyable(const CNoncopyable&);
	const CNoncopyable& operator=(const CNoncopyable&);

protected:
	CNoncopyable() {}
	virtual ~CNoncopyable() {}
};