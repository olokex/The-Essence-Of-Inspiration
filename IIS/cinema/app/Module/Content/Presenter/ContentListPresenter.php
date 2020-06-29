<?php declare(strict_types = 1);

namespace App\Module\Content\Presenter;

use App\Module\Core\Security\User;
use App\Module\Content\Factory\ContentListFactory;
use App\Module\Content\Repository\ContentRepository;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Service\AccessGuard;
use Ublaboo\DataGrid\DataGrid;

/**
 */
class ContentListPresenter extends AbstractFrontPresenter
{
    /** @var User @inject */
    public $user;

    /** @var ContentRepository @inject */
    public $contentRepository;

    /** @var ContentListFactory @inject */
    public $contentListFactory;

    protected function startup(): void
    {
        parent::startup();

        if (false === $this->user->isLoggedIn() || false === AccessGuard::hasEditorAccess($this->user->getEntity())) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    protected function createComponentContentList(): DataGrid
    {
        return $this->contentListFactory->create($this->user->getEntity());
    }

    public function handleDetail(string $id): void
    {
        $content = $this->contentRepository->getById((int) $id);
        $this->redirect(':Content:ContentDetail:default', $content);
    }
}
