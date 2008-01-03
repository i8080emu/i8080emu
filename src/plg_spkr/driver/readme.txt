  Windows драйвер дл€ обращени€ к портам спикера

¬ Windows95 и Windows98 обращатьс€ к портам спикера можно напр€мую, 
как в DOS.

¬ остальных верси€х Windows обращение к портам считаетс€ недопустимой
(привелегированой, которую не могут делать обычные приложени€) операцией.

¬ св€зи с этим возникла необходимость создани€ простого драйвера дл€ 
общени€ с портом 0x61.

»нформаци€ о написании драйвера дл€ Windows вз€та из
Microsoft Developer Network- MSDN Library - July 2002
(раздел Windows Development->Driver Development Kit->Getting Started with Windows Drivers)
