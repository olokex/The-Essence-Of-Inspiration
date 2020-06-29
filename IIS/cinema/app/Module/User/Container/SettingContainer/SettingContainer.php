<?php declare(strict_types = 1);

namespace App\Module\User\Container\SettingContainer;

use App\Module\Core\Security\User;
use App\Module\User\Container\RegistrationContainer\SettingData;
use App\Module\User\Entity\User as UserEntity;
use App\Module\User\Enum\RoleMap;
use App\Module\User\Repository\RoleRepository;
use App\Module\User\Service\AccessGuard;
use Nette\Application\UI\Form;
use Nette\Forms\Container;

/**
 */
class SettingContainer extends Container
{
    public const
        ROLE = 'role',
        FIRST_NAME = 'firstName',
        LAST_NAME = 'lastName',
        EMAIL = 'email',
        PHONE = 'phone',
        PASSWORD = 'password',
        PASSWORD_CONFIRMATION = 'passwordConfirmation';

    /** @var UserEntity */
    private $userEntity;

    /** @var RoleRepository */
    private $roleRepository;

    public function __construct(UserEntity $userEntity, RoleRepository $roleRepository, User $user)
    {
        $this->userEntity = $userEntity;
        $this->roleRepository = $roleRepository;

        if ($user->getEntity() !== $userEntity && AccessGuard::hasAdminAccess($user->getEntity())) {
            $this->addSelect(self::ROLE, '', $roleRepository->findAllForSelect())
                ->setDefaultValue($userEntity->getRole()->getId());
        }

        $this->addText(self::FIRST_NAME)
            ->setHtmlAttribute('placeholder', 'Jméno')
            ->setDefaultValue($userEntity->getFirstName())
            ->setRequired();

        $this->addText(self::LAST_NAME)
            ->setHtmlAttribute('placeholder', 'Příjmení')
            ->setDefaultValue($userEntity->getLastName())
            ->setRequired();

        $this->addEmail(self::EMAIL)
            ->setHtmlAttribute('placeholder', 'E-mail')
            ->setDefaultValue($userEntity->getEmail())
            ->setRequired();

        $this->addInteger(self::PHONE)
            ->setHtmlAttribute('placeholder', 'Telefon')
            ->setDefaultValue($userEntity->getPhone())
            ->addRule(Form::MIN, 'Délka tel. čísla musí mít alespoň 9 znaků', 9)
            ->setRequired();

        $password = $this->addPassword(self::PASSWORD)
            ->setHtmlAttribute('placeholder', 'Heslo')
            ->setNullable();

        $password->addCondition(Form::FILLED, true)
            ->addRule(Form::MIN_LENGTH, 'Heslo musí mít alespoň 5 znaků', 5)
            ->setRequired();

        $passwordConfirmation = $this->addPassword(self::PASSWORD_CONFIRMATION)
            ->setHtmlAttribute('placeholder', 'Znovu heslo');
        $passwordConfirmation
            ->addConditionOn($password, Form::FILLED, true)
            ->addRule(Form::EQUAL, 'Hesla se musejí shodovat.', $password)
            ->setRequired();
    }

    public function getValues($returnType = null): SettingData
    {
        $values = parent::getValues($returnType);

        return new SettingData(
            isset($values[self::ROLE]) ? $this->roleRepository->getByMap(RoleMap::fromScalar($values[self::ROLE])) : $this->userEntity->getRole(),
            $values[self::FIRST_NAME],
            $values[self::LAST_NAME],
            $values[self::EMAIL],
            (string) $values[self::PHONE],
            $values[self::PASSWORD]
        );
    }
}
