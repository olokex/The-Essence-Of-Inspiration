<?php declare(strict_types = 1);

namespace App\Module\User\Container\LoginContainer;

use App\Module\User\Component\LoginContainer\LoginData;
use Nette\Forms\Container;

/**
 */
class LoginContainer extends Container
{
    private const
        EMAIL = 'email',
        PASSWORD = 'password';

    public function __construct()
    {
        $this->addEmail(self::EMAIL)
            ->setHtmlAttribute('placeholder', 'E-mail')
            ->setRequired();
        $this->addPassword(self::PASSWORD)
            ->setHtmlAttribute('placeholder', 'Heslo')
            ->setRequired();
    }

    public function getValues($returnType = null): LoginData
    {
        $values = parent::getValues($returnType);

        return new LoginData($values[self::EMAIL], $values[self::PASSWORD]);
    }
}
