<?php declare(strict_types = 1);

namespace App\Module\Content\Presenter;

use App\Module\Content\Component\ContentFormControl\ContentFormControl;
use App\Module\Content\Component\ContentFormControl\IContentFormControlFactory;
use App\Module\Content\Entity\Content;
use App\Module\Content\Repository\GenreRepository;
use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Service\AccessGuard;

/**
 */
class ContentEditPresenter extends AbstractFrontPresenter
{
    /** @var User @inject */
    public $user;

    /** @var IContentFormControlFactory @inject */
    public $contentFormControlFactory;

    /** @var GenreRepository @inject */
    public $genreRepository;

    /** @var Content|null */
    private $content;

    protected function startup(): void
    {
        parent::startup();

        if (false === $this->user->isLoggedIn() || false === AccessGuard::hasEditorAccess($this->user->getEntity())) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    public function actionDefault(?Content $entity = null): void
    {
        $this->content = $entity;
    }

    public function renderDefault(): void
    {
        $this->template->content = $this->content;
    }

    protected function createComponentContentForm(): ContentFormControl
    {
        $control = $this->contentFormControlFactory->create($this->content);
        $control->onSuccess[] = function(Content $content) {
            $this->flashMessage('Informace byly uloženy.');
            $this->redirect(':Content:ContentDetail:default', $content);
        };

        return $control;
    }
}
