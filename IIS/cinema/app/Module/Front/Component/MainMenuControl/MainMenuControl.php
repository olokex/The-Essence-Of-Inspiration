<?php declare(strict_types = 1);

namespace App\Module\Front\Component\MainMenuControl;

use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Latte\TRenderable;
use App\Module\User\Entity\User as UserEntity;
use App\Module\User\Service\AccessGuard;
use Nette\Application\LinkGenerator;
use Nette\Application\UI\ITemplate;
use App\Module\Core\Security\User;

/**
 */
class MainMenuControl extends AbstractControl
{
    use TRenderable;

    /** @var User */
    private $user;

    /** @var LinkGenerator */
    private $linkGenerator;

    public function __construct(User $user, LinkGenerator $linkGenerator)
    {
        $this->user = $user;
        $this->linkGenerator = $linkGenerator;
    }

    protected function setDefaultVariables(ITemplate $template): void
    {
        parent::setDefaultVariables($template);
        $template->menuItems = $this->getMenuItems();
    }

    private function getMenuItems(): array
    {
        if ($this->user->isLoggedIn()) {
            /** @var UserEntity $userEntity */
            $userEntity = $this->user->getEntity();

            if (AccessGuard::hasEditorAccess($userEntity)) {
                $ret['Správa obsahu'] = $this->linkGenerator->link('Content:ContentList:default');
                $ret['Správa sálů'] = $this->linkGenerator->link('Hall:HallList:default');
            }

            if (AccessGuard::hasAdminAccess($userEntity)) {
                $ret['Správa uživatelů'] = $this->linkGenerator->link('User:UserList:default');
            }

            if (AccessGuard::hasCashierAccess($userEntity)) {
                $ret['Správa rezervací'] = $this->linkGenerator->link('Reservation:ReservationManagement:default');
            }

            $ret['Moje rezervace'] = $this->linkGenerator->link('Reservation:ReservationList:default', [$this->user->getEntity()]);
            $ret['Nastavení'] = $this->linkGenerator->link('User:Setting:default', [$this->user->getEntity()]);
            $ret['Odhlášení'] = $this->linkGenerator->link('User:Logout:default');
        } else {
            $ret['Přihlášení'] = $this->linkGenerator->link('User:Login:default');
            $ret['Registrace'] = $this->linkGenerator->link('User:Registration:default');
        }

        return $ret;
    }
}
